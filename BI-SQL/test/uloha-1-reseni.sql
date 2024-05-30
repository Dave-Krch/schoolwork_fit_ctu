CREATE OR REPLACE FUNCTION kolik_penez(id_zam_in in integer) RETURN number IS

    platy my_table;

    mesice INTEGER;

    plat_celkem FLOAT(64) default 0;

    datum_do DATE;
BEGIN
    SELECT my_table_row(OD, DO, PLAT) BULK COLLECT INTO platy FROM ZAM_HISTORIE WHERE ID_ZAM = id_zam_in;

    if (platy.COUNT = 0) then
        RAISE_APPLICATION_ERROR(-20001, 'Zaměstnanec s číslem ' || id_zam_in || ' buď neexistuje nebo nemá žádnou historii');
    end if;

    FOR i IN 1..platy.COUNT LOOP

        -- nastavi dnesni datum pokud je do null
        if (platy(i).do IS NULL) then
            datum_do := current_date;
        else
            datum_do := platy(i).DO;
        end if;

        mesice := round(months_between(datum_do, platy(i).od), 0);

        -- DBMS_OUTPUT.PUT_LINE('OD: ' || platy(i).od || ', DO: ' || datum_do || ', PLAT: ' || platy(i).plat || '  months  ' || mesice);

        plat_celkem := plat_celkem + (mesice * platy(i).PLAT);

    END LOOP;

    -- DBMS_OUTPUT.PUT_LINE('Plat celkem: ' || plat_celkem);

return plat_celkem;
end;