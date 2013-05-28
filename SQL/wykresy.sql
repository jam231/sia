COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 2) TO 'D://wyk2.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 3) TO 'D://wyk3.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 4) TO 'D://wyk4.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 5) TO 'D://wyk5.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 6) TO 'D://wyk6.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 7) TO 'D://wyk7.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 8) TO 'D://wyk8.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 9) TO 'D://wyk9.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 10) TO 'D://wyk10.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 11) TO 'D://wyk11.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 12) TO 'D://wyk12.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 13) TO 'D://wyk13.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 14) TO 'D://wyk14.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 15) TO 'D://wyk15.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 16) TO 'D://wyk16.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 17) TO 'D://wyk17.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 18) TO 'D://wyk18.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 19) TO 'D://wyk19.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 20) TO 'D://wyk20.txt';
COPY(SELECT EXTRACT(EPOCH FROM czas), cena FROM zrealizowane_zlecenie WHERE id_zasobu = 21) TO 'D://wyk21.txt';

COPY(SELECT * FROM wykres_uz(
	    (SELECT id_uz FROM uzytkownik WHERE (stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) =  
		(SELECT MAX(stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) FROM uzytkownik) )
		) ) TO 'D://wykbu.txt';
		
COPY(SELECT * FROM wykres_uz(
		(SELECT id_uz FROM uzytkownik WHERE (stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) =  
		(SELECT MIN(stan_konta(0, id_uz) - stan_konta(100000000, id_uz)) FROM uzytkownik) )
		) ) TO 'D://wykwu.txt';
		
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
		(SELECT EXTRACT(EPOCH FROM czas) FROM zrealizowane_zlecenie WHERE uz_kupil = uz OR uz_sprzedal = uz)
	LOOP
		r =(czas1, stan_konta(czas1, uz));
		RETURN NEXT r;
	END LOOP;
	RETURN;
END;
$$ LANGUAGE plpgsql;
