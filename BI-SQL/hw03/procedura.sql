CREATE OR REPLACE PROCEDURE predci (p_jmeno IN VARCHAR2, p_typ VARCHAR2) IS

    CURSOR predci_cursor (pc_jmeno VARCHAR2, pc_typ VARCHAR2) IS
        WITH Rodice(generace, cv, jmeno, otec, matka, pohlavi) AS (
            SELECT 0, v1.cv, v1.jmeno, v1.otec, v1.matka, v1.pohlavi
            FROM vydra v1
            WHERE v1.jmeno = pc_jmeno
            UNION ALL
            SELECT r.generace + 1,  v2.cv, v2.jmeno, v2.otec, v2.matka, v2.pohlavi
            FROM vydra v2, Rodice r
            WHERE
                (pc_typ = 'muzska' and r.otec = v2.CV) or
                (pc_typ = 'zenska' and r.matka = v2.CV) or
                (pc_typ = 'obe' and (r.otec = v2.CV or r.matka = v2.CV))
            )
        SELECT * FROM Rodice;

    predci_record predci_cursor%ROWTYPE;

    spaces VARCHAR2(200);

    vydra_existuje INTEGER;
BEGIN
    -- osetreni vstupu
    IF p_typ != 'muzska' and p_typ != 'zenska' and p_typ != 'obe'
        THEN
            RAISE_APPLICATION_ERROR(-20001, 'Spatny parametr! Mozne parametry: ''muzska'', ''zenska'', ''obe''');
    end if;

    SELECT count(*) INTO vydra_existuje FROM vydra v WHERE v.JMENO = p_jmeno;

    IF vydra_existuje = 0
        THEN
            RAISE_APPLICATION_ERROR(-20002, 'Vydra ''' || p_jmeno || ''' neexistuje');
    end if;

    -- uvodni print
    DBMS_OUTPUT.PUT_LINE('----- Rodokmen vydry ' || p_jmeno || ', typ: ' || p_typ || ' -----');

    -- cursor
    OPEN predci_cursor(p_jmeno, p_typ);

    LOOP
        FETCH predci_cursor INTO predci_record;
        EXIT WHEN predci_cursor%NOTFOUND;

        spaces := '';

        FOR i in 0..predci_record.generace LOOP
            spaces := spaces || '  ';
        END LOOP;

        DBMS_OUTPUT.PUT_LINE(spaces || '-' || predci_record.jmeno || ', generace: ' || predci_record.generace || ', pohlavi: ' || predci_record.pohlavi);

    END LOOP;

    CLOSE predci_cursor;

END;