CREATE OR REPLACE FUNCTION nowy_uzytkownik(nowe_haslo varchar(15)) RETURNS integer AS $$
DECLARE
	nowy_id integer := nextval('nr_uz');
BEGIN
	INSERT INTO uzytkownik(id_uz, haslo) VALUES(nowy_id, nowe_haslo);
	RETURN nowy_id;
END;
$$ LANGUAGE plpgsql;




CREATE OR REPLACE FUNCTION przydziel_zasoby(uz integer) RETURNS VOID AS $$ --kazdy uzytkownik dostaje JAKAS akcje oraz pewna ustalona kwote pieniedzy
DECLARE
	wartosc_pieniedzy	integer := 1000000; --10 000zl * 100 gr
	l_zasobow			integer;
	zasob				integer;
BEGIN
	l_zasobow := (SELECT COUNT(*) FROM zasob);
	INSERT INTO posiadane_dobro(id_uz,id_zasobu,ilosc) VALUES(uz,1,wartosc_pieniedzy);
	INSERT INTO posiadane_dobro(id_uz,id_zasobu,ilosc) VALUES(uz,FLOOR(RANDOM()*(l_zasobow-1))+2,100); --kazdy otrzymuje losowy zasob w ilosci 100
	END;
$$ LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION przydziel_zasoby_wszystkim() RETURNS VOID AS $$
DECLARE
	r integer;
BEGIN
	FOR r IN --petla przydzielania po wszystkich uzytkownikach
		(SELECT id_uz FROM uzytkownik)
	LOOP
		PERFORM przydziel_zasoby(r);
	END LOOP;
END;
$$ LANGUAGE plpgsql;


--------------------------------------------------------------
----TODO PONIZEJ: 
--POWIADOMIENIA DLA APLIKACJI
--PRZETESTOWAC TO
--------------------------------------------------------------




CREATE OR REPLACE FUNCTION zlecenie_kupna_on_delete() RETURNS TRIGGER AS $$
BEGIN
	--ZWROT KASY
	UPDATE posiadane_dobro SET ilosc=ilosc+old.ilosc*old.limit1 WHERE id_uz=old.id_uz AND id_zasobu=1;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zk_on_delete BEFORE DELETE on zlecenie_kupna
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_kupna_on_delete();


	
	
CREATE OR REPLACE FUNCTION zlecenie_sprzedazy_on_delete() RETURNS TRIGGER AS $$
BEGIN
	--ZWROT AKCJI
	UPDATE posiadane_dobro SET ilosc=ilosc+old.ilosc WHERE id_uz=old.id_uz AND id_zasobu=old.id_zasobu;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zs_on_delete BEFORE DELETE on zlecenie_sprzedazy
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_sprzedazy_on_delete();



	
	
	
CREATE OR REPLACE FUNCTION zlecenie_kupna_on_update() RETURNS TRIGGER AS $$
BEGIN	
	--DODAJ AKCJE:
	UPDATE posiadane_dobro SET ilosc=ilosc+(old.ilosc-new.ilosc) WHERE id_uz=old.id_uz AND id_zasobu=old.id_zasobu;
	--LOG:
	INSERT INTO zrealizowane_zlecenie(id_uz,id_zasobu,ilosc,cena) VALUES(old.id_uz,old.id_zasobu,(old.ilosc-new.ilosc),old.limit1); --czy mozna w insercie wstawiac takie wyr. arytm.?
	
	IF new.ilosc=0 THEN --jesli zlecenie jest zrealizowane to sie usuwa
		DELETE FROM zlecenie_kupna WHERE id_zlecenia=old.id_zlecenia;
	END IF;
	
	
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zk_on_update AFTER UPDATE ON zlecenie_kupna
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_kupna_on_update();

	
	
CREATE OR REPLACE FUNCTION zlecenie_sprzedazy_on_update() RETURNS TRIGGER AS $$
BEGIN	
	--DODAJ KASE:
	UPDATE posiadane_dobro SET ilosc=ilosc+(old.ilosc-new.ilosc)*old.limit1 WHERE id_uz=old.id_uz AND id_zasobu=1;
	--LOG:
	INSERT INTO zrealizowane_zlecenie(id_uz,id_zasobu,ilosc,cena) VALUES(old.id_uz,old.id_zasobu,(new.ilosc-old.ilosc),old.limit1); --czy mozna w insercie wstawiac takie wyr. arytm.?
	
	IF new.ilosc=0 THEN --jesli zlecenie jest zrealizowane to sie usuwa
		DELETE FROM zlecenie_sprzedazy WHERE id_zlecenia=old.id_zlecenia;
	END IF;
		
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zs_on_update AFTER UPDATE ON zlecenie_sprzedazy
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_sprzedazy_on_update();
	


	


--Aby zrealizowac wszystkie zlecenia z kolejki, tj. uruchomic sesje:
--PERFORM poslij_zlecenie_kupna(zlecenie_kupna.*) FROM zlecenie_kupna ORDER BY wazne_od ASC;
--na danym zasobie:
--PERFORM poslij_zlecenie_kupna(zlecenie_kupna.*) FROM zlecenie_kupna WHERE id_zasobu=[X] ORDER BY wazne_od ASC;

CREATE OR REPLACE FUNCTION wykonaj_zlecenie_kupna(rekord zlecenie_kupna) RETURNS VOID AS $$
DECLARE
	zlecenie zlecenie_sprzedazy%rowtype; --"placeholder" na zlecenie, z ktorym bedzie parowane to nasze
	ile INTEGER;
BEGIN
	ile := rekord.ilosc;
	
	LOOP
		IF ile=0 OR (SELECT COUNT(*) FROM zlecenie_sprzedazy WHERE id_zasobu=rekord.id_zasobu AND limit1<=rekord.limit1)=0 THEN --to jest aktualnie straszne. nalezy zrealizowac to w kursorach. Chyba.
			EXIT;
		END IF;
		
		--Wypelnij zawartosc zmiennej "zlecenie"
		SELECT * INTO zlecenie FROM zlecenie_sprzedazy 
			WHERE id_zasobu=rekord.id_zasobu AND limit1<=rekord.limit1 ORDER BY limit1,wazne_od ASC LIMIT 1;
		
		
		IF zlecenie.ilosc>ile THEN --jesli "zlecenie" jest wieksze niz chcemy zrealizowac, nalezy uaktualnic zlecenie.ilosc w nim
			UPDATE zlecenie_sprzedazy	SET ilosc=zlecenie.ilosc-ile WHERE id_zlecenia=zlecenie.id_zlecenia;
			ile := 0;
		ELSE
			UPDATE zlecenie_sprzedazy SET ilosc=0 WHERE id_zlecenia=zlecenie.id_zlecenia;
			ile := ile - zlecenie.ilosc;
		END IF;
		
	END LOOP;
	
	IF (ile<rekord.ilosc) THEN --cos sie sprzedalo
		UPDATE zlecenie_kupna SET ilosc=ile WHERE id_zlecenia=rekord.id_zlecenia; --zawsze "ile" powinno byc min. 0
	END IF;
	
END
$$ LANGUAGE plpgsql;



--Skopiowany kod w polowie. Pewnie do refaktoryzacji. Ale pewnie nikt tego i tak nie przeczyta i sie do mnie nie przyczepi

CREATE OR REPLACE FUNCTION wykonaj_zlecenie_sprzedazy(rekord zlecenie_sprzedazy) RETURNS VOID AS $$
DECLARE
	zlecenie zlecenie_kupna%rowtype;  --"placeholder" na zlecenie, z ktorym bedzie parowane to nasze
	ile INTEGER;
BEGIN
	ile := rekord.ilosc;
	
	LOOP
		RAISE NOTICE 'looping at%',ile;
		IF ile=0 OR (SELECT COUNT(*) FROM zlecenie_kupna WHERE id_zasobu=rekord.id_zasobu AND limit1>=rekord.limit1 AND ilosc>0)=0 THEN --to jest aktualnie straszne. nalezy zrealizowac to w kursorach. Chyba.
			EXIT;
		END IF;
		
		--Wypelnij zawartosc zmiennej "zlecenie"
		SELECT * INTO zlecenie FROM zlecenie_kupna 
			WHERE id_zasobu=rekord.id_zasobu AND limit1>=rekord.limit1 ORDER BY limit1 DESC,wazne_od ASC LIMIT 1;
		
		
		IF zlecenie.ilosc>ile THEN --jesli "zlecenie" jest wieksze niz chcemy zrealizowac, nalezy uaktualnic zlecenie.ilosc w nim
			UPDATE zlecenie_kupna	SET ilosc=zlecenie.ilosc-ile WHERE id_zlecenia=zlecenie.id_zlecenia;
			ile := 0;
		ELSE
			UPDATE zlecenie_kupna	SET ilosc=0 WHERE id_zlecenia=zlecenie.id_zlecenia;
			ile := ile - zlecenie.ilosc;
		END IF;
		
	END LOOP;
	
	IF (ile<rekord.ilosc) THEN --cos sie kupilo
		UPDATE zlecenie_sprzedazy SET ilosc=ile WHERE id_zlecenia=rekord.id_zlecenia; --zawsze "ile" powinno byc min. 0
	END IF;
	
END
$$ LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION zlecenie_kupna_on_insert() RETURNS TRIGGER AS $$
BEGIN
	--wstawianie do tabeli posiadanych dobr juz zasobu w ilosci 0. bazuja na tym update'y w przypadku realizacji zlecenia
	INSERT INTO posiadane_dobro(id_uz,id_zasobu,ilosc) --pewnie nalezaloby to w przyszlosci wywalic
       SELECT new.id_uz,new.id_zasobu,0
		WHERE NOT EXISTS (SELECT 1 FROM posiadane_dobro WHERE id_uz=new.id_uz AND id_zasobu=new.id_zasobu);
	   
	UPDATE posiadane_dobro SET ilosc=ilosc-new.ilosc*new.limit1 WHERE id_uz=new.id_uz AND id_zasobu=1;
	
	
	IF (SELECT mozna_handlowac FROM zasob WHERE id_zasobu=new.id_zasobu) THEN
		PERFORM wykonaj_zlecenie_kupna(new);
	END IF;
	
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zk_on_insert AFTER INSERT on zlecenie_kupna
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_kupna_on_insert();

	
CREATE OR REPLACE FUNCTION zlecenie_sprzedazy_on_insert() RETURNS TRIGGER AS $$
BEGIN
	UPDATE posiadane_dobro SET ilosc=ilosc-new.ilosc WHERE id_uz=new.id_uz AND id_zasobu=new.id_zasobu;
	
	IF (SELECT mozna_handlowac FROM zasob WHERE id_zasobu=new.id_zasobu) THEN
		PERFORM wykonaj_zlecenie_sprzedazy(new);
	END IF;
	
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zs_on_insert AFTER INSERT on zlecenie_sprzedazy
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_sprzedazy_on_insert();
	
	


--Z tych funkcji sie korzysta przy wstawianiu. Zwraca ID zlecenia wstawionego
CREATE OR REPLACE FUNCTION zlec_kupno(uz integer,zasob integer,ile integer,cena integer) RETURNS integer AS $$
DECLARE
	nowy_id integer := nextval('nr_zlecenia');
BEGIN
	INSERT INTO zlecenie_kupna(id_zlecenia,id_uz, id_zasobu, ilosc, limit1) VALUES(nowy_id, uz, zasob, ile,cena);
	RETURN nowy_id;
END;
$$ LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION zlec_sprzedaz(uz integer,zasob integer,ile integer,cena integer) RETURNS integer AS $$
DECLARE
	nowy_id integer := nextval('nr_zlecenia');
BEGIN
	INSERT INTO zlecenie_sprzedazy(id_zlecenia,id_uz, id_zasobu, ilosc, limit1) VALUES(nowy_id, uz, zasob, ile,cena);
	RETURN nowy_id;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION rozpocznij_sesje() RETURNS VOID AS $$
BEGIN
	UPDATE zasob SET mozna_handlowac=true;
	PERFORM poslij_zlecenie_kupna(zlecenie_kupna.*) FROM zlecenie_kupna ORDER BY wazne_od ASC;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION zakoncz_sesje() RETURNS VOID AS $$
BEGIN
	UPDATE zasob SET mozna_handlowac=false;
END;
$$ LANGUAGE plpgsql;
