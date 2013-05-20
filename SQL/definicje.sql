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
	liczba_akcji	INTEGER --na poczatku to moze byc wartosc wyliczana
);
	
--ponizej sa dwie tabele dla zlecen kupna i sprzedazy: jesli ktos uwaza to za niestosowne, mozna to zmienic w jedna.
CREATE TABLE zlecenia_kupna (
	id_zlecenia	INTEGER PRIMARY KEY DEFAULT nextval('nr_zlecenia'),
	id_uz		INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
	typ			CHAR NOT NULL,
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL,
	ilosc		INTEGER NOT NULL,
	limit1		INTEGER,
	limit2		INTEGER,
	wazne_od	DATE NOT NULL,
	wazne_do	DATE
);

CREATE TABLE zlecenia_sprzedazy (
	id_zlecenia	INTEGER PRIMARY KEY DEFAULT nextval('nr_zlecenia'),
	id_uz		INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
	typ			CHAR NOT NULL,
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL,
	ilosc		INTEGER NOT NULL,
	limit1		INTEGER,
	limit2		INTEGER,
	wazne_od	DATE NOT NULL,
	wazne_do	DATE
);

CREATE TABLE posiadane_dobra (
	id_uz		INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL,
	ilosc		INTEGER,
	PRIMARY KEY (id_uz, id_zasobu)
);

CREATE TABLE zrealizowane_zlecenia (
	uz_kupil	INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
	uz_sprzedal INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL,
	ilosc		INTEGER NOT NULL,
	czas		TIMESTAMP NOT NULL
);

CREATE TABLE subskrypcje (
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

CREATE OR REPLACE FUNCTION przydziel_zasoby() RETURNS VOID AS $$ --kazdy uzytkownik dostaje JAKAS akcje oraz pewna ustalona kwote pieniedzy
DECLARE
	wartosc_pieniedzy	integer := 1000000; --10 000zl * 100 gr
	l_zasobow			integer;
	r					integer;
BEGIN
	l_zasobow := (SELECT COUNT(*) FROM zasob);
	FOR r IN
		SELECT id_uz FROM uzytkownik
	LOOP
		INSERT INTO posiadane_dobra(id_uz,id_zasobu,ilosc) VALUES(r,1,10000);
		INSERT INTO posiadane_dobra(id_uz,id_zasobu,ilosc) VALUES(r,FLOOR(RANDOM()*l_zasobow)+1,100); --kazdy otrzymuje losowy zasob w ilosci 100
	END LOOP;
END;
$$ LANGUAGE plpgsql;
