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


CREATE OR REPLACE FUNCTION transfer_resources(buy buy_order, sell sell_order, is_purchase boolean) RETURNS INTEGER AS $$
DECLARE 
	price 				integer;
	shares_count	   integer;
BEGIN
	shares_count := LEAST(buy.amount, sell.amount);
	
	IF is_purchase = true THEN 
		price := sell.limit1;
	ELSE
		price := buy.limit1;
	END IF;
	
	--- Resource reservation is done on inserting the orders.
	UPDATE owned_stock SET amount = amount + shares_count * price 
	WHERE user_id = sell.user_id AND stock_id = 1; 					-- Give seller the money
	
	UPDATE owned_stock SET amount = amount + shares_count 
	WHERE user_id = buy.user_id AND stock_id = buy.stock_id;        -- Give buyer the shares

	UPDATE owned_stock SET amount = amount + shares_count * (buy.limit1 - price) 
	WHERE user_id = buy.user_id AND stock_id = 1; 					-- Give back part of the frozen money to the buyer.
	
	UPDATE buy_order SET amount = amount - shares_count WHERE order_id = buy.order_id;
	UPDATE sell_order SET amount = amount - shares_count WHERE order_id = sell.order_id;

	
	PERFORM pg_notify('ch_order_change', buy.user_id || '|' || buy.order_id || '|' || shares_count || '|' || price);
	PERFORM pg_notify('ch_order_change', sell.user_id || '|' || sell.order_id || '|' || shares_count || '|' || price);

	INSERT INTO transaction(buyer_id,seller_id,stock_id,amount,price) VALUES(buy.user_id, sell.user_id, buy.stock_id, shares_count, price);

	IF buy.amount = shares_count THEN
		PERFORM pg_notify('ch_order_completed', buy.user_id || '|' || buy.order_id);
	END IF;

	IF sell.amount = shares_count THEN
		PERFORM pg_notify('ch_order_completed', sell.user_id || '|' || sell.order_id);
	END IF;

	RETURN shares_count;
END
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION process_buy_order(base_order buy_order) RETURNS VOID AS $$
DECLARE
	matching_order sell_order%rowtype;
BEGIN	
	LOOP
		IF base_order.amount = 0 THEN 
			EXIT;
		END IF;

		SELECT sell_order.* INTO matching_order FROM sell_order JOIN stock USING(stock_id)
			WHERE stock_id = base_order.stock_id AND limit1 >= base_order.limit1 AND 
				  amount > 0 AND available_since <= CURRENT_TIMESTAMP AND 
				  active AND user_id != base_order.user_id 
			ORDER BY limit1, available_since LIMIT 1;

		-- Stop if there is no matching orders.
		IF matching_order.order_id IS NULL THEN
			EXIT;
		END IF;

		--Quick and dirty fix --- for what ???
		SELECT * INTO base_order FROM buy_order 
			WHERE order_id = base_order.order_id;			
		
		base_order.amount := base_order.amount - transfer_resources(base_order, matching_order, true);
		
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
		SELECT buy_order.* INTO matching_order FROM buy_order JOIN stock USING(stock_id)
		WHERE stock_id = base_order.stock_id AND limit1 >= base_order.limit1 AND 
			  amount > 0 AND available_since <= CURRENT_TIMESTAMP AND active AND
			  user_id != base_order.user_id
		ORDER BY limit1 DESC, available_since ASC 
		LIMIT 1;

		-- No matching order found.
		IF matching_order.order_id IS NULL THEN
			EXIT;
		END IF;

		--Quick and dirty fix --- for what ???
		SELECT * INTO base_order FROM sell_order 
		WHERE order_id = base_order.order_id;		

		base_order.amount := base_order.amount - transfer_resources(matching_order, base_order, false);
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
	SELECT SUM(amount) as volume, limit1 FROM buy_order WHERE buy_order.stock_id = stock_id AND amount > 0 
	GROUP BY limit1 ORDER BY 2 DESC 
	LIMIT 1 
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION best_sell_metric(in stock_id integer, out bigint, out int) AS $$ 
    SELECT SUM(amount) as volume,limit1 FROM sell_order WHERE sell_order.stock_id = stock_id AND amount > 0 
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