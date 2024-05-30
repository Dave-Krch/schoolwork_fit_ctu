/*
    creates crossjoin of ids from obchodnik and zakaznik,
    then inserts p_count rows into obchod with ids randomly selected from the crossjoin
*/

CREATE OR REPLACE PROCEDURE fill_obchod (p_count IN INTEGER) IS

    TYPE ID_TABLE_ROW IS RECORD (id_clen INTEGER, id_zakaznik INTEGER);
    TYPE ID_TABLE IS TABLE OF ID_TABLE_ROW;

    id_crossjoin ID_TABLE;

    rand_record_index INTEGER;

    rand_date DATE;
BEGIN

    SELECT o.id_clen, z.id_zakaznik
    BULK COLLECT INTO id_crossjoin
    FROM obchodnik o CROSS JOIN zakaznik z;

    FOR i IN 1..p_count LOOP
        rand_date := TO_DATE('1900-01-01', 'YYYY-MM-DD') + DBMS_RANDOM.VALUE(1, TO_DATE('2024-12-31', 'YYYY-MM-DD') - TO_DATE('1900-01-01', 'YYYY-MM-DD'));
        rand_record_index := DBMS_RANDOM.VALUE(1, id_crossjoin.COUNT);

        INSERT INTO obchod VALUES (DEFAULT, id_crossjoin(rand_record_index).id_clen, id_crossjoin(rand_record_index).id_zakaznik, rand_date);
    END LOOP;

END;