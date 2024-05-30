/*
    Inserts p_count rows into obchodnik and correesponding rows into clen
*/

CREATE OR REPLACE PROCEDURE fill_obchodnik (p_count IN INTEGER) IS

    TYPE T_NAMES IS TABLE OF VARCHAR(64);

    names T_NAMES;

    age INTEGER;
    bodycount INTEGER;

    first_name_index INTEGER;
    second_name_index INTEGER;
    alias_index INTEGER;

    composed_name VARCHAR(64);
    first_name VARCHAR(64);
    second_name VARCHAR(64);
    alias VARCHAR(64);

    v_id_clen INTEGER;

    v_iteration_count INTEGER := 0;

 -- Check if the alias already exists in the obchodnik table
    FUNCTION IsAliasUnique(p_alias VARCHAR2) RETURN BOOLEAN IS
    v_count NUMBER;
    BEGIN
        SELECT COUNT(*) INTO v_count
        FROM obchodnik
        WHERE alias = p_alias;

        RETURN v_count = 0;
    END IsAliasUnique;

BEGIN

    SELECT jmeno BULK COLLECT INTO names FROM jmena;

    FOR i IN 1..p_count LOOP
        first_name_index := DBMS_RANDOM.VALUE(1, names.COUNT);
        second_name_index := DBMS_RANDOM.VALUE(1, names.COUNT);

        age := DBMS_RANDOM.VALUE(18, 99);
        bodycount := DBMS_RANDOM.VALUE(0, 99);

        first_name := names(first_name_index);
        second_name := names(second_name_index);

        composed_name := first_name || ' ' || second_name;

        INSERT INTO clen (id_bydliste, jmeno, vek) VALUES (-1, composed_name, age) RETURNING id_clen INTO v_id_clen;

        alias_index := DBMS_RANDOM.VALUE(1, names.COUNT);
        alias := names(alias_index);

        -- finding unique alias
        WHILE v_iteration_count < names.COUNT LOOP
        alias_index := DBMS_RANDOM.VALUE(1, names.COUNT);
        alias := names(alias_index);

        IF IsAliasUnique(alias) THEN
            EXIT;
        END IF;

        v_iteration_count := v_iteration_count + 1;
    END LOOP;

        IF IsAliasUnique(alias) THEN
            INSERT INTO obchodnik VALUES (v_id_clen, alias, bodycount);
        end if;

    END LOOP;

END;