CREATE SEQUENCE user_id_seq MAXVALUE 4294967295;	-- 2^32 - 1
CREATE SEQUENCE order_id_seq MAXVALUE 4294967295;
CREATE SEQUENCE stock_id_seq MAXVALUE 4294967295;

CREATE TABLE users(
    user_id			INTEGER PRIMARY KEY,
	password		varchar(15) NOT NULL 
							CONSTRAINT more_than_4 CHECK (character_length(password) > 4)
							CONSTRAINT no_white_spaces CHECK (password !~ '_*\s_*')
);

CREATE TABLE stock(
	stock_id		INTEGER PRIMARY KEY DEFAULT nextval('stock_id_seq'),
	name			VARCHAR(50),
	book_value		INTEGER,
	profit			INTEGER,
	debt			INTEGER,
	volume			INTEGER,
	active			BOOLEAN NOT NULL DEFAULT TRUE
);
	
CREATE TABLE buy_order(
	order_id		INTEGER PRIMARY KEY DEFAULT nextval('order_id_seq'),
	user_id			INTEGER REFERENCES users(user_id) NOT NULL,
	type			CHAR NOT NULL DEFAULT 'z', --ordinary order
	stock_id		INTEGER REFERENCES stock(stock_id) NOT NULL,
	amount			INTEGER NOT NULL,
	limit1			INTEGER NOT NULL,
	limit2			INTEGER,
	available_since	TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	available_until	TIMESTAMP

	CONSTRAINT 	c_bo_stock_id CHECK (stock_id > 1)
	CONSTRAINT	c_boamount	CHECK (amount >= 0),
	CONSTRAINT	c_bolimit1	CHECK (limit1 >= 0)
);

CREATE TABLE sell_order(
	order_id		INTEGER PRIMARY KEY DEFAULT nextval('order_id_seq'),
	user_id			INTEGER REFERENCES users(user_id) NOT NULL,
	type			CHAR NOT NULL DEFAULT 'z', --ordinary order
	stock_id		INTEGER REFERENCES stock(stock_id) NOT NULL,
	amount			INTEGER NOT NULL,
	limit1			INTEGER NOT NULL,
	limit2			INTEGER,
	available_since	TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	available_until	TIMESTAMP

	CONSTRAINT 	c_so_stock_id CHECK (stock_id > 1)
	CONSTRAINT	c_so_amount	CHECK (amount >= 0),
	CONSTRAINT	c_so_limit1	CHECK (limit1 >= 0)
);

CREATE TABLE owned_stock(
	user_id		INTEGER REFERENCES users(user_id) NOT NULL,
	stock_id	INTEGER REFERENCES stock(stock_id) NOT NULL,
	amount		INTEGER NOT NULL

	CONSTRAINT	c_ro_amount	CHECK (amount >= 0),
	PRIMARY KEY (user_id, stock_id)
);


CREATE TABLE transaction(
	time		TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	stock_id	INTEGER REFERENCES stock(stock_id) NOT NULL,
	buyer_id	INTEGER REFERENCES users(user_id) NOT NULL, 
	seller_id 	INTEGER REFERENCES users(user_id) NOT NULL, 
	amount		INTEGER NOT NULL,
	price		INTEGER NOT NULL

	CONSTRAINT	wt_amount	CHECK (amount >= 0)
);

CREATE VIEW market_value AS
	SELECT stock_id,(book_value::bigint*10000000)/volume AS price FROM stock WHERE stock_id > 1;
	
CREATE INDEX so1_idx ON sell_order(user_id);
CREATE INDEX bo1_idx ON buy_order(user_id);
CREATE INDEX ro1_idx ON owned_stock(user_id);

-- For efficiency of wykonaj_sell_order and wykonaj_buy_order.
CREATE INDEX so2_idx ON sell_order (order_id, limit1, available_since);
CREATE INDEX bo2_idx ON buy_order (order_id ASC, limit1 DESC, available_since	 ASC);