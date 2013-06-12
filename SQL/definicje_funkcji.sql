CREATE OR REPLACE FUNCTION przydziel_zasoby(uz integer) RETURNS VOID AS $$ --kazdy uzytkownik dostaje JAKAS akcje oraz pewna ustalona kwote pieniedzy
DECLARE
	wartosc_pieniedzy	integer := 1000000; --10 000zl * 100 gr
	l_zasobow			integer;
	l_zasobow_dla_uz	integer;
	zasob				integer;
	kasa_w_akcjach		integer;
	wartosc_zasobu		integer;
	id_zas				integer;
	przydzielone_id 	INTEGER ARRAY := array[1::integer];
	val					NUMERIC;
BEGIN
	l_zasobow := (SELECT COUNT(*) FROM zasob);
	l_zasobow_dla_uz := (random()::numeric*5)::integer+1;
	kasa_w_akcjach := (random()*10000000)::integer; -- poki co po prostu liczba akcji :(
	
	INSERT INTO posiadane_dobro(id_uz,id_zasobu,ilosc) VALUES(uz,1,wartosc_pieniedzy);
	
	FOREACH val IN ARRAY podziel_odcinek(l_zasobow_dla_uz) LOOP
		LOOP
			id_zas := FLOOR(RANDOM()*(l_zasobow-1))+2;
			IF NOT(przydzielone_id @> array[id_zas]) THEN
				przydzielone_id := przydzielone_id || id_zas;
				EXIT;
			END IF;
		END LOOP;
		wartosc_zasobu := (SELECT cena FROM cena_rynkowa WHERE id_zasobu = id_zas);
		--RAISE NOTICE  'WZ=% VAL=% KWA=% IA=%', wartosc_zasobu, val, kasa_w_akcjach, (val*kasa_w_akcjach/wartosc_zasobu)::int;
		INSERT INTO posiadane_dobro(id_uz,id_zasobu,ilosc) VALUES(uz,id_zas,(val*kasa_w_akcjach/wartosc_zasobu)::int ); --kazdy otrzymuje losowy zasob
	END LOOP;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION numeric_min(a numeric, b numeric) RETURNS NUMERIC AS $$
BEGIN
	IF a>b THEN RETURN b; END IF;
	RETURN a;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION podziel_odcinek(na_ile_czesci integer) RETURNS NUMERIC ARRAY AS $$
DECLARE
	retval NUMERIC ARRAY;
	single_value NUMERIC;
	max_val NUMERIC := 0.5;
	total NUMERIC := 1.0;
BEGIN
	FOR i IN 2..na_ile_czesci LOOP
		max_val := numeric_min(max_val, total);
		single_value := (random()::NUMERIC) % max_val;
		retval := retval||(single_value);
		total := total - single_value;
	END LOOP;
	retval := retval||total;
	RETURN retval;
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



CREATE OR REPLACE FUNCTION nowy_uzytkownik(nowe_haslo varchar(15)) RETURNS integer AS $$
DECLARE
	nowy_id integer := nextval('nr_uz');
BEGIN
	INSERT INTO uzytkownik(id_uz, haslo) VALUES(nowy_id, nowe_haslo);
	PERFORM przydziel_zasoby(nowy_id);
	RETURN nowy_id;
END;
$$ LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION zlecenie_kupna_on_delete() RETURNS TRIGGER AS $$
BEGIN
	--ZWROT KASY
	UPDATE posiadane_dobro SET ilosc=ilosc+old.ilosc*old.limit1 WHERE id_uz=old.id_uz AND id_zasobu=1;
	RETURN old;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zk_on_delete BEFORE DELETE on zlecenie_kupna
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_kupna_on_delete();


	
	
CREATE OR REPLACE FUNCTION zlecenie_sprzedazy_on_delete() RETURNS TRIGGER AS $$
BEGIN
	--ZWROT AKCJI
	UPDATE posiadane_dobro SET ilosc=ilosc+old.ilosc WHERE id_uz=old.id_uz AND id_zasobu=old.id_zasobu;
	RETURN old;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zs_on_delete BEFORE DELETE on zlecenie_sprzedazy
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_sprzedazy_on_delete();

	
	

CREATE OR REPLACE FUNCTION val_min(a integer,b integer) RETURNS INTEGER AS $$
BEGIN
	IF (a>=b) THEN
		RETURN b;
	ELSE
		RETURN a;
	END IF;
END
$$ LANGUAGE plpgsql;
	
	
	
CREATE OR REPLACE FUNCTION przenies_dobra(zl_kupna zlecenie_kupna, zl_sprzedazy zlecenie_sprzedazy, kupno boolean) RETURNS INTEGER AS $$
DECLARE 
	cena integer;
	ile  integer;
BEGIN
	ile := val_min(zl_kupna.ilosc, zl_sprzedazy.ilosc);
	
	IF kupno=true THEN --najpierw byla sprzedaz. dopasowalo sie kupno
		cena := zl_sprzedazy.limit1;
	ELSE
		cena := zl_kupna.limit1;
	END IF;
	
	--UWAGA, nie zabieramy nic nikomu, bo to robi sie juz przy momencie wstawienia zlecenia
	UPDATE posiadane_dobro SET ilosc=ilosc+ile*cena WHERE id_uz=zl_sprzedazy.id_uz AND id_zasobu=1; --DODAJ KASE SPRZEDAJACEMU
	UPDATE posiadane_dobro SET ilosc=ilosc+ile WHERE id_uz=zl_kupna.id_uz AND id_zasobu=zl_kupna.id_zasobu;        --DODAJ ZASOB KUPUJACEMU

	UPDATE posiadane_dobro SET ilosc=ilosc+ile*(zl_kupna.limit1-cena) WHERE id_uz=zl_kupna.id_uz AND id_zasobu=1; --kupujacemu oddaj ew. pieniadze
	
	--RAISE NOTICE 'ile = % old_cached_ilosc = % old_ilosc = %',ile,zl_kupna.ilosc,(SELECT ilosc FROM zlecenie_kupna WHERE id_zlecenia=zl_kupna.id_zlecenia);
	UPDATE zlecenie_kupna SET ilosc=ilosc-ile WHERE id_zlecenia=zl_kupna.id_zlecenia;
	UPDATE zlecenie_sprzedazy SET ilosc=ilosc-ile WHERE id_zlecenia=zl_sprzedazy.id_zlecenia;
	
	INSERT INTO zrealizowane_zlecenie(uz_kupil,uz_sprzedal,id_zasobu,ilosc,cena) VALUES(zl_kupna.id_uz, zl_sprzedazy.id_uz, zl_kupna.id_zasobu, ile, cena);

	--RAISE NOTICE 'sold %', ile;
	RETURN ile;
END
$$ LANGUAGE plpgsql;

--Aby zrealizowac wszystkie zlecenia z kolejki, tj. uruchomic sesje:
--PERFORM wykonaj_zlecenie_kupna(zlecenie_kupna.*) FROM zlecenie_kupna ORDER BY wazne_od ASC;
--na danym zasobie:
--PERFORM wykonaj_zlecenie_kupna(zlecenie_kupna.*) FROM zlecenie_kupna WHERE id_zasobu=[X] ORDER BY wazne_od ASC;

CREATE OR REPLACE FUNCTION wykonaj_zlecenie_kupna(rekord zlecenie_kupna) RETURNS VOID AS $$
DECLARE
	zlecenie zlecenie_sprzedazy%rowtype; --"placeholder" na zlecenie, z ktorym bedzie parowane to nasze
	ile INTEGER;
BEGIN
	ile := rekord.ilosc;
	
	LOOP
		--RAISE NOTICE 'k looping at % stock is % (%)',ile, rekord.id_zasobu, (SELECT COUNT(*) FROM zlecenie_sprzedazy WHERE id_zasobu=rekord.id_zasobu AND limit1<=rekord.limit1 AND ilosc>0);
		IF ile=0 OR (SELECT COUNT(*) FROM zlecenie_sprzedazy WHERE id_zasobu=rekord.id_zasobu AND limit1<=rekord.limit1 AND ilosc>0)=0 THEN --to jest aktualnie straszne. nalezy zrealizowac to w kursorach. Chyba.
			EXIT;
		END IF;
		
		--Wypelnij zawartosc zmiennej "zlecenie"
		SELECT * INTO zlecenie FROM zlecenie_sprzedazy 
			WHERE id_zasobu=rekord.id_zasobu AND limit1<=rekord.limit1 AND ilosc>0 ORDER BY limit1,wazne_od ASC LIMIT 1;
			
		--Quick and dirty fix
		SELECT * INTO rekord FROM zlecenie_kupna 
			WHERE id_zlecenia=rekord.id_zlecenia;			
		ile := ile - przenies_dobra(rekord, zlecenie, true);
		
	END LOOP;
END
$$ LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION wykonaj_zlecenie_sprzedazy(rekord zlecenie_sprzedazy) RETURNS VOID AS $$
DECLARE
	zlecenie zlecenie_kupna%rowtype;  --"placeholder" na zlecenie, z ktorym bedzie parowane to nasze
	ile INTEGER;
BEGIN
	ile := rekord.ilosc;
	
	LOOP
		--RAISE NOTICE 's looping at%',ile;
		IF ile=0 OR (SELECT COUNT(*) FROM zlecenie_kupna WHERE id_zasobu=rekord.id_zasobu AND limit1>=rekord.limit1 AND ilosc>0)=0 THEN --to jest aktualnie straszne. nalezy zrealizowac to w kursorach. Chyba.
			EXIT;
		END IF;
		
		--Wypelnij zawartosc zmiennej "zlecenie"
		SELECT * INTO zlecenie FROM zlecenie_kupna 
			WHERE id_zasobu=rekord.id_zasobu AND limit1>=rekord.limit1 AND ilosc>0 ORDER BY limit1 DESC,wazne_od ASC LIMIT 1;

		--Quick and dirty fix
		SELECT * INTO rekord FROM zlecenie_sprzedazy 
			WHERE id_zlecenia=rekord.id_zlecenia;				
		ile := ile - przenies_dobra(zlecenie, rekord, false);
		
	END LOOP;
	
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
	
	
CREATE OR REPLACE FUNCTION zrealizowane_zlecenie_on_insert() RETURNS TRIGGER AS $$
BEGIN
	PERFORM pg_notify('ch_zmiana',new.id_zasobu||'|'||new.ilosc||'|'||new.cena||'|'||new.czas);
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zz_on_insert AFTER INSERT on zrealizowane_zlecenie
	FOR EACH ROW EXECUTE PROCEDURE zrealizowane_zlecenie_on_insert();
	
	

--Z tych funkcji sie korzysta przy wstawianiu. Zwraca ID zlecenia wstawionego
CREATE OR REPLACE FUNCTION zlec_kupno(uz integer,zasob integer,ile integer,cena integer) RETURNS integer AS $$
DECLARE
	nowy_id integer := nextval('nr_zlecenia');
BEGIN
    --RAISE NOTICE 'zlec_kupno u: % z: % i: % c: %',uz,zasob,ile,cena;
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
	PERFORM wykonaj_zlecenie_kupna(zlecenie_kupna.*) FROM zlecenie_kupna ORDER BY wazne_od ASC;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION zakoncz_sesje() RETURNS VOID AS $$
BEGIN
	UPDATE zasob SET mozna_handlowac=false;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION zlecenie_kupna_on_update() RETURNS TRIGGER AS $$
BEGIN
	PERFORM pg_notify('ch_zlecenia_kupna',new.id_zlecenia||'|'||new.ilosc||'|'||new.id_uz);
	IF new.ilosc=0 THEN --jesli zlecenie jest zrealizowane to sie usuwa
		DELETE FROM zlecenie_kupna WHERE id_zlecenia=new.id_zlecenia;
	END IF;
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zk_on_update AFTER UPDATE ON zlecenie_kupna
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_kupna_on_update();

	
	
CREATE OR REPLACE FUNCTION zlecenie_sprzedazy_on_update() RETURNS TRIGGER AS $$
BEGIN
	PERFORM pg_notify('ch_zlecenia_sprzedazy',new.id_zlecenia||'|'||new.ilosc||'|'||new.id_uz);
	
	IF new.ilosc=0 THEN --jesli zlecenie jest zrealizowane to sie usuwa
		DELETE FROM zlecenie_sprzedazy WHERE id_zlecenia=new.id_zlecenia;
	END IF;
		
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zs_on_update AFTER UPDATE ON zlecenie_sprzedazy
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_sprzedazy_on_update();
	
	
CREATE OR REPLACE FUNCTION najlepsze_kupno(in zasob integer, out integer, out bigint)
    AS $$ SELECT limit1,SUM(ilosc) FROM zlecenie_kupna t WHERE id_zasobu=zasob AND ilosc>0 GROUP BY limit1 ORDER BY 1 DESC LIMIT 1 $$
LANGUAGE SQL;



CREATE OR REPLACE FUNCTION najlepsza_sprzedaz(in zasob integer, out integer, out bigint)
    AS $$ SELECT limit1,SUM(ilosc) FROM zlecenie_sprzedazy t WHERE id_zasobu=zasob AND ilosc>0 GROUP BY limit1 ORDER BY 1 ASC LIMIT 1 $$
LANGUAGE SQL;



CREATE OR REPLACE FUNCTION ostatnia_transakcja(in zasob integer, out integer, out integer, out timestamp without time zone)
    AS $$ SELECT cena,ilosc,czas FROM zrealizowane_zlecenie t WHERE id_zasobu=zasob ORDER BY czas ASC LIMIT 1 $$
LANGUAGE SQL;



CREATE OR REPLACE FUNCTION dobra_uz(uz integer) RETURNS TABLE(id_zasobu integer,ilosc integer)
	AS $$ SELECT id_zasobu, ilosc FROM posiadane_dobro WHERE id_uz=uz AND (ilosc>0 OR id_zasobu=1) $$
LANGUAGE SQL;


CREATE OR REPLACE FUNCTION transakcje_uz(uz integer,ile integer) RETURNS SETOF zrealizowane_zlecenie AS
$$
	SELECT * FROM zrealizowane_zlecenie WHERE uz_kupil=uz OR uz_sprzedal=uz ORDER BY czas DESC LIMIT ile;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION transakcje_na_zasobie(id integer) RETURNS SETOF zrealizowane_zlecenie AS
$$
	SELECT * from zrealizowane_zlecenie WHERE id_zasobu = id ORDER BY czas DESC;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION zlecenia_uz(uz integer) RETURNS TABLE(typ integer, id_zlecenia integer, id_zasobu integer,ilosc integer, limit1 integer)
	AS $$ 
	SELECT 1, id_zlecenia, id_zasobu, ilosc, limit1 FROM zlecenie_kupna WHERE id_uz=uz
	UNION
	SELECT 2, id_zlecenia, id_zasobu, ilosc, limit1 FROM zlecenie_sprzedazy WHERE id_uz=uz;
	$$
LANGUAGE SQL;