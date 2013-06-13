create type punkt as (x double precision, y integer);	

CREATE OR REPLACE FUNCTION stan_konta(czas1 double precision, uz integer) RETURNS integer AS $$
DECLARE
	sprzedal integer;
	kupil integer;
BEGIN
	sprzedal = (SELECT SUM(ilosc*cena) FROM zrealizowane_zlecenie WHERE EXTRACT(EPOCH FROM czas) >= czas1 AND uz_sprzedal = uz);
	IF sprzedal is NULL THEN 
		sprzedal = 0;
	END IF;
	
	kupil = (SELECT SUM(ilosc*cena) FROM zrealizowane_zlecenie WHERE EXTRACT(EPOCH FROM czas) >= czas1 AND uz_kupil = uz);
	IF kupil is NULL THEN 
		kupil = 0;
	END IF;
	
	RETURN (SELECT ilosc FROM posiadane_dobro WHERE id_zasobu = 1 AND id_uz = uz) + sprzedal - kupil;
END;
$$ LANGUAGE plpgsql;
	
CREATE OR REPLACE FUNCTION wykres_uz(uz integer) RETURNS setof punkt AS $$
DECLARE
	czas1 integer;
	r punkt%ROWTYPE;
BEGIN
	FOR czas1 IN
		(SELECT EXTRACT(EPOCH FROM czas) FROM zrealizowane_zlecenie WHERE uz_kupil = uz OR uz_sprzedal = uz ORDER BY czas DESC)
	LOOP
		r =(czas1, stan_konta(czas1, uz));
		RETURN NEXT r;
	END LOOP;
	RETURN;
END;
$$ LANGUAGE plpgsql;

CREATE VIEW ostatnie_ceny AS
	select distinct zz.id_zasobu,zz.cena from zrealizowane_zlecenie zz join 
	(SELECT id_zasobu, max(czas) as czas FROM zrealizowane_zlecenie group by id_zasobu) t on (zz.id_zasobu=t.id_zasobu and zz.czas=t.czas);

CREATE VIEW szacowana_wartosc AS
	SELECT id_uz, SUM(ilosc::bigint*cena) FROM posiadane_dobro as pd JOIN ostatnie_ceny as oc USING(id_zasobu) WHERE
		ilosc>0 group by id_uz;

CREATE OR REPLACE FUNCTION utworz_wykresy() RETURNS VOID AS $$
BEGIN
	FOR i IN 2..21 LOOP
		EXECUTE 'COPY(SELECT EXTRACT(EPOCH FROM czas), cena, SUM(ilosc) FROM zrealizowane_zlecenie WHERE id_zasobu = '||i||' GROUP BY czas,cena ORDER BY czas) TO ''D://wyk'||i||'.txt''';
	END LOOP;
	COPY(SELECT * FROM szacowana_wartosc ORDER BY 2 DESC) TO 'D://dane.txt';
		
	--COPY(SELECT * FROM wykres_uz(
	--    (SELECT id_uz FROM uzytkownik WHERE (stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) =  
	--	(SELECT MAX(stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) FROM uzytkownik LIMIT 1) )
	--	) ) TO 'D://wykbu.txt';
		
	--COPY(SELECT * FROM wykres_uz(
	--	(SELECT id_uz FROM uzytkownik WHERE (stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) =  
	--	(SELECT MIN(stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) FROM uzytkownik LIMIT 1) )
	--	) ) TO 'D://wykwu.txt';
		
END;
$$ LANGUAGE plpgsql;
