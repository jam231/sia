CREATE SEQUENCE nr_uz MAXVALUE 4294967295;	-- 2^32 - 1
CREATE SEQUENCE nr_zlecenia;
CREATE SEQUENCE nr_zasobu;

CREATE TABLE uzytkownik (
    id_uz		INTEGER PRIMARY KEY,
	haslo		varchar(15) NOT NULL 
							CONSTRAINT more_than_4 CHECK (character_length(haslo) > 4)
							CONSTRAINT no_white_spaces CHECK (haslo !~ '_*\s_*')
);

CREATE TABLE zasob (
	id_zasobu		INTEGER PRIMARY KEY DEFAULT nextval('nr_zasobu'),
	nazwa			VARCHAR(50),
	w_ksiegowa		INTEGER, --na poczatku moze byc puste, docelowo to realna wycena spolki sciagnieta z internetu
	zysk			INTEGER,
	dlug			INTEGER,
	liczba_akcji	INTEGER, --na poczatku to moze byc wartosc wyliczana
	mozna_handlowac	BOOLEAN NOT NULL
);
	
--ponizej sa dwie tabele dla zlecen kupna i sprzedazy: jesli ktos uwaza to za niestosowne, mozna to zmienic w jedna.
CREATE TABLE zlecenie_kupna (
	id_zlecenia	INTEGER PRIMARY KEY DEFAULT nextval('nr_zlecenia'),
	id_uz		INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
	typ			CHAR NOT NULL DEFAULT 'z', --zwykle
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL,
	ilosc		INTEGER NOT NULL,
	limit1		INTEGER NOT NULL,
	limit2		INTEGER,
	wazne_od	TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	wazne_do	TIMESTAMP
);

CREATE TABLE zlecenie_sprzedazy (
	id_zlecenia	INTEGER PRIMARY KEY DEFAULT nextval('nr_zlecenia'),
	id_uz		INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
	typ			CHAR NOT NULL DEFAULT 'z', --zwykle
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL,
	ilosc		INTEGER NOT NULL,
	limit1		INTEGER NOT NULL,
	limit2		INTEGER,
	wazne_od	TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	wazne_do	TIMESTAMP
);

CREATE TABLE posiadane_dobro (
	id_uz		INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL,
	ilosc		INTEGER,
	PRIMARY KEY (id_uz, id_zasobu)
);


CREATE TABLE zrealizowane_zlecenie (
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL, --po tym chcemy indeksowac najpierw
	id_uz		INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
--	uz_kupil	INTEGER REFERENCES uzytkownik(id_uz), --NOT NULL tymczasowo usuniete ze wzgledu na problemy z odzyskiwaniem tego
--	uz_sprzedal INTEGER REFERENCES uzytkownik(id_uz), --NOT NULL ... j/w
	ilosc		INTEGER NOT NULL,
	cena		INTEGER NOT NULL,
	czas		TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE subskrypcja (
	id_uz		INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL
);





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
BEGIN
	l_zasobow := (SELECT COUNT(*) FROM zasob);
	INSERT INTO posiadane_dobro(id_uz,id_zasobu,ilosc) VALUES(uz,1,wartosc_pieniedzy);
	INSERT INTO posiadane_dobro(id_uz,id_zasobu,ilosc) VALUES(uz,FLOOR(RANDOM()*l_zasobow)+1,100); --kazdy otrzymuje losowy zasob w ilosci 100
END;
$$ LANGUAGE plpgsql;







CREATE OR REPLACE FUNCTION przydziel_zasoby_wszystkim() RETURNS VOID AS $$
DECLARE
	r integer;
BEGIN
	FOR r IN
		(SELECT id_uz FROM uzytkownik)
	LOOP
		PERFORM przydziel_zasoby(r);
	END LOOP;
END;
$$ LANGUAGE plpgsql;



--TODO PONIZEJ: POWIADOMIENIA DLA APLIKACJI

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
	UPDATE posiadane_dobro SET ilosc=ilosc+(new.ilosc-old.ilosc) WHERE id_uz=old.id_uz AND id_zasobu=old.id_zasobu;
	--LOG:
	INSERT INTO zrealizowane_zlecenie(id_uz,id_zasobu,ilosc,cena) VALUES(old.id_uz,old.id_zasobu,(new.ilosc-old.ilosc),old.limit1); --czy mozna w insercie wstawiac takie wyr. arytm.?
	
	IF new.ilosc=0 THEN --jesli zlecenie jest zrealizowane to sie usuwa
		DELETE FROM zlecenie_sprzedazy WHERE id_zlecenia=old.id_zlecenia;
	END IF;
	
	
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zk_on_update AFTER UPDATE ON zlecenie_kupna
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_kupna_on_update();

	
	
--duplikacja kodu +/-
CREATE OR REPLACE FUNCTION zlecenie_sprzedazy_on_update() RETURNS TRIGGER AS $$
BEGIN	
	--DODAJ KASE:
	UPDATE posiadane_dobro SET ilosc=ilosc+(new.ilosc-old.ilosc)*old.limit1 WHERE id_uz=old.id_uz AND id_zasobu=1;
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
--PERFORM poslij_zlecenie_kupna(zlecenie_kupna.*) FROM zlecenie_kupna;

--Chyba bede musial zrobic symetryczna funkcje dla sprzedazy, nie wiem czy to brzydkie, czy nie
CREATE OR REPLACE FUNCTION wykonaj_zlecenie_kupna(rekord zlecenie_kupna) RETURNS VOID AS $$
DECLARE
	zlecenie zlecenie_sprzedazy%rowtype;
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
			UPDATE zlecenie_sprzedazy	SET ilosc=zlecenie.ilosc-ile WHERE id_zlecenia=zlecenie.id;
			ile := 0;
		ELSE
			UPDATE zlecenie_sprzedazy SET ilosc=0 WHERE id_zlecenia=zlecenie.id;
			ile := ile - zlecenie.ilosc;
		END IF;
		
	END LOOP;
	
	IF (ile<rekord.ilosc) THEN --cos sie sprzedalo
		UPDATE zlecenie_kupna SET ilosc=ile WHERE id_zlecenia=rekord.id_zlecenia; --zawsze "ile" powinno byc min. 0
	END IF;
	
END
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION zlecenie_kupna_on_insert() RETURNS TRIGGER AS $$
BEGIN
	INSERT INTO posiadane_dobro(id_uz,id_zasobu,ilosc) --pewnie nalezaloby to w przyszlosci wywalic
       SELECT new.id_uz,new.id_zasobu,0
		WHERE NOT EXISTS (SELECT 1 FROM posiadane_dobro WHERE id_ud=new.id_uz AND id_zasobu=new.id_zasobu);
	   
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
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zs_on_insert AFTER INSERT on zlecenie_sprzedazy
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_sprzedazy_on_insert();