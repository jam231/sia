
--				 DELETE TRIGGERS

CREATE OR REPLACE FUNCTION buy_order_on_delete() RETURNS TRIGGER AS $$
BEGIN
	-- return money to the buyer
	UPDATE owned_stock SET amount=amount+old.amount*old.limit1 WHERE user_id=old.user_id AND stock_id=1;
	RETURN old;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER bo_on_delete BEFORE DELETE on buy_order
	FOR EACH ROW EXECUTE PROCEDURE buy_order_on_delete();

	
CREATE OR REPLACE FUNCTION sell_order_on_delete() RETURNS TRIGGER AS $$
BEGIN
	--return shares to the seller
	UPDATE owned_stock SET amount=amount+old.amount WHERE user_id=old.user_id AND stock_id=old.stock_id;
	RETURN old;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER so_on_delete BEFORE DELETE on sell_order
	FOR EACH ROW EXECUTE PROCEDURE sell_order_on_delete();
	
--				 INSERT TRIGGERS

CREATE OR REPLACE FUNCTION buy_order_on_insert() RETURNS TRIGGER AS $$
BEGIN
	-- some functions for managing order processing (helper_procedures.sql) rely on buyer owning stock he is buying  
	INSERT INTO owned_stock(user_id,stock_id,amount)
    SELECT new.user_id,new.stock_id,0
		WHERE NOT EXISTS (SELECT 1 FROM owned_stock WHERE user_id=new.user_id AND stock_id=new.stock_id);
	   
	UPDATE owned_stock SET amount=amount-new.amount*new.limit1 WHERE user_id=new.user_id AND stock_id=1;
	
	IF (SELECT active FROM stock WHERE stock_id=new.stock_id) THEN
		PERFORM wykonaj_buy_order(new);
	END IF;
	
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER bo_on_insert AFTER INSERT on buy_order
	FOR EACH ROW EXECUTE PROCEDURE buy_order_on_insert();

	
CREATE OR REPLACE FUNCTION sell_order_on_insert() RETURNS TRIGGER AS $$
BEGIN
	UPDATE owned_stock SET amount=amount-new.amount WHERE user_id=new.user_id AND stock_id=new.stock_id;
	
	IF (SELECT active FROM stock WHERE stock_id=new.stock_id) THEN
		PERFORM wykonaj_zlecenie_sprzedazy(new);
	END IF;
	
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER so_on_insert AFTER INSERT on sell_order
	FOR EACH ROW EXECUTE PROCEDURE sell_order_on_insert();
	
	
CREATE OR REPLACE FUNCTION transaction_on_insert() RETURNS TRIGGER AS $$
BEGIN
	PERFORM pg_notify('ch_last_transaction_change',new.stock_id||'|'||new.amount||'|'||new.price||'|'||new.time);
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER tr_on_insert AFTER INSERT on transaction
	FOR EACH ROW EXECUTE PROCEDURE transaction_on_insert();
	
--				 UPDATE TRIGGERS

CREATE OR REPLACE FUNCTION buy_order_on_update() RETURNS TRIGGER AS $$
BEGIN
	IF new.amount=0 THEN --if order is completed, i.e. have amount = 0 then delete it from buy_order table
		DELETE FROM buy_order WHERE order_id=new.order_id;
	END IF;
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER bo_on_update AFTER UPDATE ON buy_order
	FOR EACH ROW EXECUTE PROCEDURE buy_order_on_update();

		
CREATE OR REPLACE FUNCTION sell_order_on_update() RETURNS TRIGGER AS $$
BEGIN
	IF new.amount=0 THEN --if order is completed, i.e. have amount = 0 then delete it from buy_order table
		DELETE FROM sell_order WHERE order_id=new.order_id;
	END IF;
		
	RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER so_on_update AFTER UPDATE ON sell_order
	FOR EACH ROW EXECUTE PROCEDURE sell_order_on_update();
	