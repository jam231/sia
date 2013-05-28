DELETE FROM zasob;
SELECT setval('nr_zasobu',1,false);
INSERT INTO zasob(nazwa,w_ksiegowa,zysk,liczba_akcji) VALUES
('PLN',1,0,0),
('ACP',7482,358,83000303),
('LWP',2366,260,34013590),
('BRS',1196,19,2256715692),
('BRE',9978,1195,42141986),
('EUR',11240,772,441442578),
('GTC',2997,35,319372990),
('BHW',7562,1079,130659600),
('JSW',8719,633,117411596),
('KER',8719,633,117411596),
('KGH',23460,4556,200000000),
('LTS',8750,179,129873362),
('PEO',24032,2914,262470034),
('PGE',42351,2788,1869760829),
('PGN',28411,3022,4100000000),
('PKN',28564,1233,427709061),
('PKO',25422,3532,1250000000),
('PZU',14952,3278,86352300),
('SNS',3064,485,1323250000),
('TPE',17316,1626,1752549394),
('TPS',13045,693,1335649021);

SELECT nowy_uzytkownik('abcdef'); --szef wszystkich szefow
INSERT INTO posiadane_dobro(id_uz,id_zasobu,ilosc)
	SELECT 1,id_zasobu,1000000000 FROM zasob;
	
