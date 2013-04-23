CREATE SEQUENCE nr_uz;
CREATE SEQUENCE nr_zlecenia;
CREATE SEQUENCE nr_zasobu;

CREATE TABLE uzytkownik (
    id_uz       INTEGER      PRIMARY KEY
);

CREATE TABLE zasob (
	id_zasobu	INTEGER PRIMARY KEY DEFAULT nextval('nr_zasobu'),
	nazwa		VARCHAR(50),
	w_ksiegowa	INTEGER, --na poczatku moze byc puste, docelowo to realna wycena spolki sciagnieta z internetu
	zysk		INTEGER,
	dlug		INTEGER,
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
	id_uz		INTEGER REFERENCES uzytkownik(id_uz)	NOT NULL,
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu)		NOT NULL,
	ilosc		INTEGER,
	PRIMARY KEY (id_uz, id_zasobu)
);

CREATE TABLE zrealizowane_zlecenia (
	uz_kupil	INTEGER REFERENCES uzytkownik(id_uz)	NOT NULL,
	uz_sprzedal INTEGER REFERENCES uzytkownik(id_uz)	NOT NULL,
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL,
	ilosc		INTEGER NOT NULL,
	czas		TIMESTAMP NOT NULL
);