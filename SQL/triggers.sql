
--				 DELETE TRIGGERS

CREATE OR REPLACE FUNCTION buy_order_on_delete() RETURNS TRIGGER AS $$
BEGIN
	--ZWROT KASY
	UPDATE owned_stock SET amount=amount+old.amount*old.limit1 WHERE user_id=old.user_id AND stock_id=1;
	RETURN old;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER bo_on_delete BEFORE DELETE on buy_order
	FOR EACH ROW EXECUTE PROCEDURE buy_order_on_delete();

	
CREATE OR REPLACE FUNCTION zlecenie_sprzedazy_on_delete() RETURNS TRIGGER AS $$
BEGIN
	--ZWROT AKCJI
	UPDATE owned_stock SET amount=amount+old.amount WHERE user_id=old.user_id AND stock_id=old.stock_id;
	RETURN old;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zs_on_delete BEFORE DELETE on sell_order
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_sprzedazy_on_delete();
	
--				 INSERT TRIGGERS

CREATE OR REPLACE FUNCTION zlecenie_kupna_on_insert() RETURNS TRIGGER AS $$
BEGIN
	--wstawianie do tabeli posiadanych dobr juz stocku w amounti 0. bazuja na tym update'y w przypadku realizacji zlecenia
	INSERT INTO owned_stock(user_id,stock_id,amount) --pewnie nalezaloby to w przyszlosci wywalic
    SELECT new.user_id,new.stock_id,0
		WHERE NOT EXISTS (SELECT 1 FROM owned_stock WHERE user_id=new.user_id AND stock_id=new.stock_id);
	   
	UPDATE owned_stock SET amount=amount-new.amount*new.limit1 WHERE user_id=new.user_id AND stock_id=1;
	
	
	IF (SELECT mozna_handlowac FROM stock WHERE stock_id=new.stock_id) THEN
		PERFORM wykonaj_zlecenie_kupna(new);
	END IF;
	
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zk_on_insert AFTER INSERT on buy_order
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_kupna_on_insert();

	
CREATE OR REPLACE FUNCTION zlecenie_sprzedazy_on_insert() RETURNS TRIGGER AS $$
BEGIN
	UPDATE owned_stock SET amount=amount-new.amount WHERE user_id=new.user_id AND stock_id=new.stock_id;
	
	IF (SELECT active FROM stock WHERE stock_id=new.stock_id) THEN
		PERFORM wykonaj_zlecenie_sprzedazy(new);
	END IF;
	
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zs_on_insert AFTER INSERT on sell_order
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_sprzedazy_on_insert();
	
	
CREATE OR REPLACE FUNCTION zrealizowane_zlecenie_on_insert() RETURNS TRIGGER AS $$
BEGIN
	PERFORM pg_notify('ch_last_transaction_change',new.stock_id||'|'||new.amount||'|'||new.price||'|'||new.time);
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zz_on_insert AFTER INSERT on transaction
	FOR EACH ROW EXECUTE PROCEDURE zrealizowane_zlecenie_on_insert();
	
--				 UPDATE TRIGGERS

CREATE OR REPLACE FUNCTION zlecenie_kupna_on_update() RETURNS TRIGGER AS $$
BEGIN
	IF new.amount=1 THEN --jesli zlecenie jest zrealizowane to sie usuwa
		DELETE FROM buy_order WHERE order_id=new.order_id;
	END IF;
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zk_on_update AFTER UPDATE ON buy_order
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_kupna_on_update();

		
CREATE OR REPLACE FUNCTION zlecenie_sprzedazy_on_update() RETURNS TRIGGER AS $$
BEGIN
	IF new.amount=1 THEN --jesli zlecenie jest zrealizowane to sie usuwa
		DELETE FROM sell_order WHERE order_id=new.order_id;
	END IF;
		
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER zs_on_update AFTER UPDATE ON sell_order
	FOR EACH ROW EXECUTE PROCEDURE zlecenie_sprzedazy_on_update();
	