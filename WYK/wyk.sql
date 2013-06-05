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

CREATE OR REPLACE FUNCTION utworz_wykresy() RETURNS VOID AS $$
BEGIN
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 1 ORDER BY czas) TO 'D://wyk1.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 2 ORDER BY czas) TO 'D://wyk2.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 3 ORDER BY czas) TO 'D://wyk3.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 4 ORDER BY czas) TO 'D://wyk4.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 5 ORDER BY czas) TO 'D://wyk5.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 6 ORDER BY czas) TO 'D://wyk6.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 7 ORDER BY czas) TO 'D://wyk7.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 8 ORDER BY czas) TO 'D://wyk8.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 9 ORDER BY czas) TO 'D://wyk9.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 10 ORDER BY czas) TO 'D://wyk10.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 11 ORDER BY czas) TO 'D://wyk11.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 12 ORDER BY czas) TO 'D://wyk12.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 13 ORDER BY czas) TO 'D://wyk13.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 14 ORDER BY czas) TO 'D://wyk14.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 15 ORDER BY czas) TO 'D://wyk15.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 16 ORDER BY czas) TO 'D://wyk16.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 17 ORDER BY czas) TO 'D://wyk17.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 18 ORDER BY czas) TO 'D://wyk18.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 19 ORDER BY czas) TO 'D://wyk19.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 20 ORDER BY czas) TO 'D://wyk20.txt';
	COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 21 ORDER BY czas) TO 'D://wyk21.txt';
	
	COPY(SELECT * FROM wykres_uz(
	    (SELECT id_uz FROM uzytkownik WHERE (stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) =  
		(SELECT MAX(stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) FROM uzytkownik LIMIT 1) )
		) ) TO 'D://wykbu.txt';
		
	COPY(SELECT * FROM wykres_uz(
		(SELECT id_uz FROM uzytkownik WHERE (stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) =  
		(SELECT MIN(stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) FROM uzytkownik LIMIT 1) )
		) ) TO 'D://wykwu.txt';
	
END;
$$ LANGUAGE plpgsql;
