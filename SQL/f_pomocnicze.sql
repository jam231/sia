CREATE OR REPLACE FUNCTION stworz_uzytkownikow(ile integer) RETURNS VOID AS $$
BEGIN
FOR i IN 2..(ile) LOOP
	PERFORM nowy_uzytkownik( RIGHT('00000000' || CAST(i AS VARCHAR),6));
END LOOP;
END
$$ LANGUAGE plpgsql;