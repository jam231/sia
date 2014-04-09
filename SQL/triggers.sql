
--				 DELETE TRIGGERS

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
	
--				 INSERT TRIGGERS

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
	PERFORM pg_notify('ch_last_transaction_change',new.id_zasobu||'|'||new.ilosc||'|'||new.cena||'|'||new.czas);
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zz_on_insert AFTER INSERT on zrealizowane_zlecenie
	FOR EACH ROW EXECUTE PROCEDURE zrealizowane_zlecenie_on_insert();
	
--				 UPDATE TRIGGERS

CREATE OR REPLACE FUNCTION zlecenie_kupna_on_update() RETURNS TRIGGER AS $$
BEGIN
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
	IF new.ilosc=0 THEN --jesli zlecenie jest zrealizowane to sie usuwa
		DELETE FROM zlecenie_sprzedazy WHERE id_zlecenia=new.id_zlecenia;
	END IF;
		
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zs_on_update AFTER UPDATE ON zlecenie_sprzedazy
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_sprzedazy_on_update();
	