BEGIN
    EXECUTE IMMEDIATE 'DELETE FROM "CLEN"';
    EXECUTE IMMEDIATE 'DELETE FROM "OBCHODNIK"';
    EXECUTE IMMEDIATE 'DELETE FROM "VZTAH"';
    EXECUTE IMMEDIATE 'DELETE FROM "BLIZKA_OSOBA"';
    EXECUTE IMMEDIATE 'DELETE FROM "BYDLISTE"';

    EXECUTE IMMEDIATE 'ALTER SEQUENCE "CLEN_SEQ" RESTART start with 1';
    EXECUTE IMMEDIATE 'ALTER SEQUENCE "BLIZKA_OSOBA_SEQ" RESTART start with 1';
    EXECUTE IMMEDIATE 'ALTER SEQUENCE "BYDLISTE_SEQ" RESTART start with 1';
END;

-- zakladni hodnoty
INSERT INTO BYDLISTE (id_bydliste, stat, mesto, ulice, cislo_domu) VALUES (default, 'placeholder', 'placeholder', 'placeholder', 0);

INSERT INTO CLEN (jmeno, vek) VALUES ('Clen s blizkymi osobami 1', 22);
INSERT INTO CLEN (jmeno, vek) VALUES ('Clen s blizkymi osobami 2', 11);
INSERT INTO CLEN (jmeno, vek) VALUES ('Clen bez blizkych osob', 99);

INSERT INTO BLIZKA_OSOBA VALUES (default, 1, 'placeholder', 99, '?');
INSERT INTO BLIZKA_OSOBA VALUES (default, 1, 'placeholder', 99, '?');

INSERT INTO VZTAH VALUES (1, 1, 'rodic');
INSERT INTO VZTAH VALUES (1, 2, 'rodic');
INSERT INTO VZTAH VALUES (2, 1, 'rodic');
INSERT INTO VZTAH VALUES (2, 2, 'rodic');

commit;

BEGIN
    -- Insert obchodnika s blizkymi osobami
    "obchodnik_wrapper".INSERT_(1, 'alias 1', 1);
END;

BEGIN
    -- Insert obchodnika bez blizkych osob
    "obchodnik_wrapper".INSERT_(3, 'alias', 99);
END;

BEGIN
   -- Dovolene updaty
    "obchodnik_wrapper".UPDATE_(1, 'novy_alias_1', 1);

   -- Zmena id na jineho clena se znamymi
    "obchodnik_wrapper".UPDATE_ID(1,2);

   -- Zmena id zpet
    "obchodnik_wrapper".UPDATE_ALL(2, 1, 'dalsi_alias', 1);
END;

BEGIN
   -- Nedovoleny update id
    "obchodnik_wrapper".UPDATE_ID(1,3);
END;
BEGIN
   -- Nedovoleny update id
    "obchodnik_wrapper".UPDATE_ALL(1, 3, 'dalsi_alias', 1);
END;
BEGIN
   -- Neexistujici obchodnik
    "obchodnik_wrapper".UPDATE_ID(-1,-1);
END;


BEGIN
    -- Delete
    "obchodnik_wrapper".DELETE_(1);
END;