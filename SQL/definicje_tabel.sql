CREATE SEQUENCE nr_uz MAXVALUE 4294967295;	-- 2^32 - 1
CREATE SEQUENCE nr_zlecenia MAXVALUE 4294967295;
CREATE SEQUENCE nr_zasobu MAXVALUE 4294967295;

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
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL, --po tym chcemy indeksowac najpierw...chyba
	id_uz		INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
--	uz_kupil	INTEGER REFERENCES uzytkownik(id_uz), --NOT NULL usuniete ze wzgledu na problemy z odzyskiwaniem tego
--	uz_sprzedal INTEGER REFERENCES uzytkownik(id_uz), --NOT NULL ... j/w
	ilosc		INTEGER NOT NULL,
	cena		INTEGER NOT NULL,
	czas		TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE subskrypcja (
	id_uz		INTEGER REFERENCES uzytkownik(id_uz) NOT NULL,
	id_zasobu	INTEGER REFERENCES zasob(id_zasobu) NOT NULL
);