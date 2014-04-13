CREATE OR REPLACE FUNCTION partition_segment(parts_count integer) RETURNS NUMERIC ARRAY AS $$
DECLARE
	retval NUMERIC ARRAY;
	single_value NUMERIC;
	max_val NUMERIC := 0.5;
	total NUMERIC := 1.0;
BEGIN
	FOR i IN 2..parts_count LOOP
		max_val := LEAST(max_val, total);
		single_value := (random()::NUMERIC) % max_val;
		retval := retval||(single_value);
		total := total - single_value;
	END LOOP;
	retval := retval||total;
	RETURN retval;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION distribute_stocks_to(uz integer) RETURNS VOID AS $$ --kazdy user dostaje JAKAS akcje oraz pewna ustalona kwote pieniedzy
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
	l_zasobow := (SELECT COUNT(*) FROM stock);
	l_zasobow_dla_uz := (random()::numeric*5)::integer+1;
	kasa_w_akcjach := (random()*10000000)::integer; -- poki co po prostu liczba akcji :(
	
	INSERT INTO owned_stock(user_id,stock_id,amount) VALUES(uz,1,wartosc_pieniedzy);
	
	FOREACH val IN ARRAY partition_segment(l_zasobow_dla_uz) LOOP
		LOOP
			id_zas := FLOOR(RANDOM()*(l_zasobow-1))+2;
			IF NOT(przydzielone_id @> array[id_zas]) THEN
				przydzielone_id := przydzielone_id || id_zas;
				EXIT;
			END IF;
		END LOOP;
		wartosc_zasobu := (SELECT price FROM market_value WHERE stock_id = id_zas);
		--RAISE NOTICE  'WZ=% VAL=% KWA=% IA=%', wartosc_zasobu, val, kasa_w_akcjach, (val*kasa_w_akcjach/wartosc_zasobu)::int;
		INSERT INTO owned_stock(user_id,stock_id,amount) VALUES(uz,id_zas,(val*kasa_w_akcjach/wartosc_zasobu)::int ); --kazdy otrzymuje losowy zasob
	END LOOP;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION distribute_stocks() RETURNS VOID AS $$
DECLARE
	r integer;
BEGIN
	FOR r IN --petla przydzielania po wszystkich userach
		(SELECT user_id FROM users)
	LOOP
		PERFORM distribute_stocks_to(r);
	END LOOP;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION get_available_stocks() RETURNS TABLE(stock_id integer) 
AS $$ SELECT stock_id FROM stock WHERE stock_id != 0; $$ LANGUAGE SQL; 



CREATE OR REPLACE FUNCTION nowy_uzytkownik(new_password varchar(15)) RETURNS integer AS $$
DECLARE
	nowy_id integer := nextval('user_id_seq');
BEGIN
	INSERT INTO users(user_id, password) VALUES(nowy_id, new_password);
	PERFORM distribute_stocks_to(nowy_id);
	RETURN nowy_id;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION przenies_dobra(zl_kupna buy_order, zl_sprzedazy sell_order, kupno boolean) RETURNS INTEGER AS $$
DECLARE 
	cena integer;
	ile  integer;
BEGIN
	ile := LEAST(zl_kupna.amount, zl_sprzedazy.amount);
	
	IF kupno=true THEN --najpierw byla sprzedaz. dopasowalo sie kupno
		cena := zl_sprzedazy.limit1;
	ELSE
		cena := zl_kupna.limit1;
	END IF;
	
	--UWAGA, nie zabieramy nic nikomu, bo to robi sie juz przy momencie wstawienia zlecenia
	UPDATE owned_stock SET amount=amount+ile*cena WHERE user_id=zl_sprzedazy.user_id AND stock_id=1; --DODAJ KASE SPRZEDAJACEMU
	UPDATE owned_stock SET amount=amount+ile WHERE user_id=zl_kupna.user_id AND stock_id=zl_kupna.stock_id;        --DODAJ ZASOB KUPUJACEMU

	UPDATE owned_stock SET amount=amount+ile*(zl_kupna.limit1-cena) WHERE user_id=zl_kupna.user_id AND stock_id=1; --kupujacemu oddaj ew. pieniadze
	
	--RAISE NOTICE 'ile = % old_cached_ilosc = % old_ilosc = %',ile,zl_kupna.ilosc,(SELECT ilosc FROM buy_order WHERE order_id=zl_kupna.order_id); 
	UPDATE buy_order SET amount=amount-ile WHERE order_id=zl_kupna.order_id;
	UPDATE sell_order SET amount=amount-ile WHERE order_id=zl_sprzedazy.order_id;

	
	PERFORM pg_notify('ch_order_change', zl_kupna.user_id || '|' || zl_kupna.order_id || '|' || ile || '|' || cena);
	PERFORM pg_notify('ch_order_change', zl_sprzedazy.user_id || '|' || zl_sprzedazy.order_id || '|' || ile  || '|' || cena);

	INSERT INTO transaction(buyer_id,seller_id,stock_id,amount,price) VALUES(zl_kupna.user_id, zl_sprzedazy.user_id, zl_kupna.stock_id, ile, cena);

	IF zl_kupna.amount-ile = 0 THEN
		PERFORM pg_notify('ch_order_completed', zl_kupna.user_id || '|' || zl_kupna.order_id);
	END IF;

	IF zl_sprzedazy.amount-ile = 0 THEN
		PERFORM pg_notify('ch_order_completed', zl_sprzedazy.user_id || '|' || zl_sprzedazy.order_id);
	END IF;
	--RAISE NOTICE 'sold %', ile;
	RETURN ile;
END
$$ LANGUAGE plpgsql;

--Aby zrealizowac wszystkie zlecenia z kolejki, tj. uruchomic sesje:
--PERFORM wykonaj_zlecenie_kupna(buy_order.*) FROM buy_order ORDER BY wazne_od ASC;
--na danym zasobie:
--PERFORM wykonaj_zlecenie_kupna(buy_order.*) FROM buy_order WHERE stock_id=[X] ORDER BY wazne_od ASC;

CREATE OR REPLACE FUNCTION process_buy_order(rekord buy_order) RETURNS VOID AS $$
DECLARE
	zlecenie sell_order%rowtype; --"placeholder" na zlecenie, z ktorym bedzie parowane to nasze
	ile INTEGER;
BEGIN
	ile := rekord.amount;
	
	LOOP
		--RAISE NOTICE 'k looping at % stock is % (%)',ile, rekord.stock_id, (SELECT COUNT(*) FROM sell_order WHERE stock_id=rekord.stock_id AND limit1<=rekord.limit1 AND ilosc>0);
		IF ile = 0 THEN 
			EXIT;
		END IF;
		
		--Wypelnij zawartosc zmiennej "zlecenie"
		-- We must do it each iteration because of concurrent nature of db computation.
		SELECT sell_order.* INTO zlecenie FROM sell_order JOIN stock USING(stock_id)
			WHERE stock_id = rekord.stock_id AND limit1 >= rekord.limit1 AND amount > 0 AND 
				  available_since <= CURRENT_TIMESTAMP AND active 
			ORDER BY limit1, available_since LIMIT 1;

		IF zlecenie.order_id IS NULL THEN
			EXIT;
		END IF;

		--Quick and dirty fix --- for what ???
		SELECT * INTO rekord FROM buy_order 
			WHERE order_id=rekord.order_id;			
		
		ile := ile - przenies_dobra(rekord, zlecenie, true);
		
	END LOOP;
END
$$ LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION process_sell_order(rekord sell_order) RETURNS VOID AS $$
DECLARE
	zlecenie buy_order%rowtype;  --"placeholder" na zlecenie, z ktorym bedzie parowane to nasze
	ile INTEGER;
BEGIN
	ile := rekord.amount;
	LOOP
		--RAISE NOTICE 's looping at%',ile;
		IF ile = 0 THEN 
			EXIT;
		END IF;
		
		--Wypelnij zawartosc zmiennej "zlecenie"
		-- We must do it each iteration because of concurrent nature of db computation.
		SELECT * INTO zlecenie FROM buy_order 
			WHERE stock_id = rekord.stock_id AND limit1 >= rekord.limit1 AND 
				  amount > 0 AND available_since <= CURRENT_TIMESTAMP AND active
			ORDER BY limit1 DESC, available_since ASC LIMIT 1;

		IF zlecenie.order_id IS NULL THEN
			EXIT;
		END IF;

		--Quick and dirty fix --- for what ???
		SELECT * INTO rekord FROM sell_order 
			WHERE order_id=rekord.order_id;		

		ile := ile - przenies_dobra(zlecenie, rekord, false);
	END LOOP;
END
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION process_buy_orders() RETURNS VOID AS $$
BEGIN
	PERFORM process_buy_order(buy_order.*) 
		FROM buy_order WHERE available_since <= CURRENT_TIMESTAMP;
END
$$ LANGUAGE plpgsql;

--Z tych funkcji sie korzysta przy wstawianiu. Zwraca ID zlecenia wstawionego
CREATE OR REPLACE FUNCTION zlec_kupno(uz integer,zasob integer,ile integer,cena integer) RETURNS integer AS $$
DECLARE
	nowy_id integer := nextval('order_id_seq');
BEGIN
    --RAISE NOTICE 'zlec_kupno u: % z: % i: % c: %',uz,zasob,ile,cena;
	INSERT INTO buy_order(order_id,user_id, stock_id, amount, limit1) VALUES(nowy_id, uz, zasob, ile,cena);
	RETURN nowy_id;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION zlec_sprzedaz(uz integer,zasob integer,ile integer,cena integer) RETURNS integer AS $$
DECLARE
	nowy_id integer := nextval('order_id_seq');
BEGIN
	INSERT INTO sell_order(order_id, user_id, stock_id, amount, limit1) VALUES(nowy_id, uz, zasob, ile,cena);
	RETURN nowy_id;
END;
$$ LANGUAGE plpgsql;

-- 							Session management

CREATE OR REPLACE FUNCTION rozpocznij_sesje() RETURNS VOID AS $$
BEGIN
	LOCK TABLE stock IN ACCESS EXCLUSIVE MODE;	-- Without it postgres deadlocks- e.g. when orders are still being processed when session changes status
	UPDATE stock SET active=true;
	--- Async - I don't care about the result.
	PERFORM dblink_connect('start_session_conn', 'dbname=' || CURRENT_DATABASE());
	PERFORM dblink_send_query('start_session_conn', 'select process_buy_orders();');
	PERFORM dblink_disconnect('start_session_conn');
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION zakoncz_sesje() RETURNS VOID AS $$
BEGIN
	LOCK TABLE stock IN ACCESS EXCLUSIVE MODE; 	-- Without it postgres deadlocks- e.g. when orders are still being processed when session changes status
	UPDATE stock SET active=false;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION najlepsze_kupno(in zasob integer, out bigint, out int)
    AS $$ SELECT SUM(amount),limit1 FROM buy_order WHERE stock_id=zasob AND amount>0 GROUP BY limit1 ORDER BY 2 DESC LIMIT 1 $$
LANGUAGE SQL;



CREATE OR REPLACE FUNCTION najlepsza_sprzedaz(in zasob integer, out bigint, out int)
    AS $$ SELECT SUM(amount),limit1 FROM sell_order WHERE stock_id=zasob AND amount>0 GROUP BY limit1 ORDER BY 2 ASC LIMIT 1 $$
LANGUAGE SQL;



CREATE OR REPLACE FUNCTION ostatnia_transakcja(in zasob integer, out integer, out integer, out timestamp without time zone)
    AS $$ SELECT price,amount,time FROM transaction WHERE stock_id=zasob ORDER BY time ASC LIMIT 1 $$
LANGUAGE SQL;



CREATE OR REPLACE FUNCTION dobra_uz(uz integer) RETURNS TABLE(stock_id integer, amount integer)
	AS $$ SELECT stock_id, amount FROM owned_stock WHERE user_id=uz AND (amount>0 OR stock_id=1) $$
LANGUAGE SQL;


CREATE OR REPLACE FUNCTION transakcje_uz(uz_id integer,ile integer) RETURNS SETOF transaction AS
$$
	SELECT * FROM transaction WHERE buyer_id=uz_id OR seller_id=uz_id ORDER BY time DESC LIMIT ile;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION transakcje_na_zasobie(id integer) RETURNS SETOF transaction AS
$$
	SELECT * from transaction WHERE stock_id = id ORDER BY time DESC;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION zlecenia_uz(uz integer) RETURNS TABLE(type integer, order_id integer, stock_id integer, amount integer, limit1 integer)
	AS $$ 
	SELECT 1, order_id, stock_id, amount, limit1 FROM buy_order WHERE user_id=uz
	UNION
	SELECT 2, order_id, stock_id, amount, limit1 FROM sell_order WHERE user_id=uz;
	$$
LANGUAGE SQL;