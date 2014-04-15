--	It generates array [x_1, x_2, ..., x_(parts_count)] for which
--
--		x_1 + x_2 + ... + x_(parts_count) = 1.0, where forall i. x_i in [0.1, 1.0)
--
-- 	x_i are chosen randomly.
CREATE OR REPLACE FUNCTION generate_random_partition(parts_count integer) RETURNS NUMERIC ARRAY AS $$
DECLARE
	parts 			 NUMERIC ARRAY;
	max_numerator  	 INTEGER := parts_count * 10;		-- arbitrary, it defines the amplitude of x_i values . 
	numerator 	 	 INTEGER := 0;						-- for example with max_numerator = 2 and parts_count = 2 there are only 3  
	denominator 	 INTEGER := 0;						-- such partitions: [0.3333, 0.66666], [0.5, 0.5], [0.666666, 0.333333]
BEGIN  
	FOR i IN 1..parts_count LOOP
		--- Generate
		numerator = (random() * max_numerator)::INTEGER + 1;
		parts := parts || (numerator :: NUMERIC);
		denominator := denominator + numerator;
	END LOOP;
	-- Map parts array with function (\numerator -> numerator/denominator)
	FOR i IN 1..parts_count LOOP
		parts[i] = parts[i] / denominator;
	END LOOP;
	
	RETURN parts;
END;
$$ LANGUAGE plpgsql;

-- Gives user a (hardcoded - 1_000_000) constant amount of money, 
-- picks randomly (from range [1,6]) how many stocks user will get. Next it chooses randomly shares money equivalent from [0, 10_000_000)
-- Lastly, for every picked stock user gets amount of its shares with equal worth to randomly picked fraction of shares money equivalent. 
-- Overall owned shares worth will not exceed 10_000_000.  
CREATE OR REPLACE FUNCTION distribute_stocks_to(user_id integer) RETURNS VOID AS $$ 
DECLARE
	money_to_distribute				integer := 1000000; --10 000zl * 100 gr
	overall_stocks_count			integer;
	user_owned_stocks_count			integer;
	shares_equivalent_in_money		integer;
	stock_market_value				integer;
	candidate_stock_id				integer;
	distributed_stock_ids 			INTEGER ARRAY := array[1::integer];
	part							NUMERIC;
BEGIN
	overall_stocks_count 			:= (SELECT COUNT(*) FROM stock WHERE stock_id != 1); -- stock_id=1 is money
	user_owned_stocks_count 		:= (random() * 6)::integer + 1;
	shares_equivalent_in_money 		:= (random() * 10000000)::integer; 		-- how much worth in shares user gets.
	
	INSERT INTO owned_stock(user_id,stock_id,amount) VALUES(user_id,1,money_to_distribute);
	
	FOREACH part IN ARRAY generate_random_partition(user_owned_stocks_count) LOOP
		-- Is there no function for generating random combination ? Or ruby's sample-like function ? 
		-- It generates random sequence of length user_owned_stocks_count of stock_ids without duplicates 
		LOOP
			candidate_stock_id := (random() * overall_stocks_count) :: integer + 2;		-- 0.0 <= random() < 1.0 
			IF NOT(distributed_stock_ids @> array[candidate_stock_id]) THEN
				distributed_stock_ids := distributed_stock_ids || candidate_stock_id;
				EXIT;
			END IF;
		END LOOP;
		
		stock_market_value := (SELECT price FROM market_value WHERE stock_id = candidate_stock_id);

		INSERT INTO owned_stock(user_id,stock_id,amount) 
			VALUES(user_id, candidate_stock_id, (part * shares_equivalent_in_money / stock_market_value)::int);
	END LOOP;
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
			WHERE stock_id = rekord.stock_id AND limit1 >= rekord.limit1 AND 
				  amount > 0 AND available_since <= CURRENT_TIMESTAMP AND 
				  active AND matching_order.user_id != base_order.user_id 
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



CREATE OR REPLACE FUNCTION process_sell_order(base_order sell_order) RETURNS VOID AS $$
DECLARE
	matching_order buy_order%rowtype;
BEGIN
	LOOP
		IF base_order.amount = 0 THEN 
			EXIT;
		END IF;
		
		-- Pick matching order for reduction.
		SELECT * INTO matching_order FROM buy_order JOIN stock USING(stock_id)
		WHERE stock_id = base_order.stock_id AND limit1 >= base_order.limit1 AND 
			  amount > 0 AND available_since <= CURRENT_TIMESTAMP AND active AND
			  matching_order.user_id != base_order.user_id
		ORDER BY limit1 DESC, available_since ASC 
		LIMIT 1;

		-- No matching order found.
		IF matching_order.order_id IS NULL THEN
			EXIT;
		END IF;

		--Quick and dirty fix --- for what ???
		SELECT * INTO base_order FROM sell_order 
		WHERE order_id = base_order.order_id;		

		base_order.amount := base_order.amount - przenies_dobra(matching_order, base_order, false);
	END LOOP;
END
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION process_buy_orders() RETURNS VOID AS $$
BEGIN
	PERFORM process_buy_order(buy_order.*) 
		FROM buy_order WHERE available_since <= CURRENT_TIMESTAMP;
END
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION create_buy_order(user_id integer, stock_id integer, amount integer, price integer) RETURNS integer AS $$
DECLARE
	order_id integer := nextval('order_id_seq');
BEGIN
	INSERT INTO buy_order(order_id,user_id, stock_id, amount, limit1) VALUES(order_id, user_id, stock_id, amount, price);
	RETURN order_id;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION create_sell_order(user_id integer, stock_id integer, amount integer, price integer) RETURNS integer AS $$
DECLARE
	order_id integer := nextval('order_id_seq');
BEGIN
	INSERT INTO sell_order(order_id, user_id, stock_id, amount, limit1) VALUES(order_id, user_id, stock_id, amount, price);
	RETURN order_id;
END;
$$ LANGUAGE plpgsql;

-- 							Session management

CREATE OR REPLACE FUNCTION begin_session() RETURNS VOID AS $$
BEGIN
	LOCK TABLE stock IN ACCESS EXCLUSIVE MODE;	-- Without it postgres deadlocks- e.g. when orders are 
												-- still being processed when session changes status
	UPDATE stock SET active=true;
	--- Async - I don't care about the result.
	PERFORM dblink_connect('start_session_conn', 'dbname=' || CURRENT_DATABASE());
	PERFORM dblink_send_query('start_session_conn', 'select process_buy_orders();');
	PERFORM dblink_disconnect('start_session_conn');
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION end_session() RETURNS VOID AS $$
BEGIN
	LOCK TABLE stock IN ACCESS EXCLUSIVE MODE; 	-- Without it postgres deadlocks, e.g. when orders are 
												-- still being processed when session changes status
	UPDATE stock SET active = false;
END;
$$ LANGUAGE plpgsql;



--						Varia


CREATE OR REPLACE FUNCTION available_stocks() RETURNS TABLE(stock_id integer) AS $$ 
	SELECT stock_id FROM stock WHERE stock_id != 0; 
$$ LANGUAGE SQL; 


CREATE OR REPLACE FUNCTION create_user(password varchar(15)) RETURNS integer AS $$
DECLARE
	user_id integer := nextval('user_id_seq');
BEGIN
	INSERT INTO users(user_id, password) VALUES(user_id, password);
	PERFORM distribute_stocks_to(user_id);
	RETURN user_id;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION best_buy_metric(in stock_id integer, out bigint, out int) AS $$ 
	SELECT SUM(amount), limit1 FROM buy_order WHERE buy_order.stock_id = stock_id AND amount > 0 
	GROUP BY limit1 ORDER BY 2 DESC 
	LIMIT 1 
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION best_sell_metric(in stock_id integer, out bigint, out int) AS $$ 
    SELECT SUM(amount),limit1 FROM sell_order WHERE sell_order.stock_id = stock_id AND amount > 0 
    GROUP BY limit1 ORDER BY 2 ASC 
    LIMIT 1 
$$ LANGUAGE SQL;



CREATE OR REPLACE FUNCTION last_transaction_for_stock(in stock_id integer, out integer, out integer, out timestamp without time zone) AS $$ 
	SELECT price, amount, time FROM transaction WHERE transaction.stock_id = stock_id 
    ORDER BY time ASC 
    LIMIT 1
$$ LANGUAGE SQL;



CREATE OR REPLACE FUNCTION user_resources(user_id integer) RETURNS TABLE(stock_id integer, amount integer) AS $$ 
	SELECT stock_id, amount FROM owned_stock 
	WHERE owned_stock.user_id = user_id AND (amount > 0 OR stock_id = 1) 
$$ LANGUAGE SQL;


CREATE OR REPLACE FUNCTION user_transactions(user_id integer, how_many integer) RETURNS SETOF transaction AS $$
	SELECT * FROM transaction WHERE buyer_id = user_id OR seller_id = user_id 
	ORDER BY time DESC 
	LIMIT how_many;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION transactions_for_stock(stock_id integer) RETURNS SETOF transaction AS $$
	SELECT * from transaction WHERE transaction.stock_id = stock_id 
	ORDER BY time DESC;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION user_orders(user_id integer) RETURNS TABLE(type integer, order_id integer, stock_id integer, amount integer, limit1 integer) AS $$ 
	SELECT 1, order_id, stock_id, amount, limit1 FROM buy_order WHERE buy_order.user_id = user_id
	UNION
	SELECT 2, order_id, stock_id, amount, limit1 FROM sell_order WHERE sell_order.user_id = user_id;
$$ LANGUAGE SQL;