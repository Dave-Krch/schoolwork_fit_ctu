/*
Inserts p_count rows into zakaznik table
*/
CREATE OR REPLACE PROCEDURE fill_zakaznik (p_count IN INTEGER) IS

    TYPE T_NAMES IS TABLE OF VARCHAR(64);

    names T_NAMES;
    countries T_NAMES;

    first_name_index INTEGER;
    second_name_index INTEGER;
    country_index INTEGER;

    composed_name VARCHAR(64);
    first_name VARCHAR(64);
    second_name VARCHAR(64);
    country VARCHAR(64);

BEGIN

    SELECT jmeno BULK COLLECT INTO names FROM jmena;
    SELECT zeme BULK COLLECT INTO countries FROM ZEME;

    FOR i IN 1..p_count LOOP
        first_name_index := DBMS_RANDOM.VALUE(1, names.COUNT);
        second_name_index := DBMS_RANDOM.VALUE(1, names.COUNT);
        country_index := DBMS_RANDOM.VALUE(1, countries.COUNT);

        first_name := names(first_name_index);
        second_name := names(second_name_index);

        composed_name := first_name || ' ' || second_name;

        country := countries(country_index);

        -- DBMS_OUTPUT.PUT_LINE('Inserting' || composed_name || ', ' || country);

        INSERT INTO zakaznik (jmeno, zeme_pusobeni) VALUES (composed_name, country);

    END LOOP;

end fill_zakaznik;