DELETE FROM zasob;
SELECT setval('nr_zasobu',1,false);
INSERT INTO zasob(nazwa) VALUES
('PLN'),
('ACP'),('BHW'),('PEO'),('BRS'),('BRE'),('GTC'),('LTS'),('JSW'),('KER'),('KGH'),('LWP'),('PKN'),('PKO'),('PGE'),('PGN'),('PZU'),('SNS'),('TPE'),('TPS'),('EUR');