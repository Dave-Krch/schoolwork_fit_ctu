BEGIN
    EXECUTE IMMEDIATE 'DELETE FROM "CLEN"';
    EXECUTE IMMEDIATE 'DELETE FROM "OBCHODNIK"';
    EXECUTE IMMEDIATE 'DELETE FROM "ZAKAZNIK"';
    EXECUTE IMMEDIATE 'DELETE FROM "OBCHOD"';
    EXECUTE IMMEDIATE 'DELETE FROM "TERORISTICKA_ORGANIZACE"';

    EXECUTE IMMEDIATE 'ALTER SEQUENCE "CLEN_SEQ" RESTART start with 1';
    EXECUTE IMMEDIATE 'ALTER SEQUENCE "OBCHOD_SEQ" RESTART start with 1';
    EXECUTE IMMEDIATE 'ALTER SEQUENCE "ZAKAZNIK_SEQ" RESTART start with 1';
END;

commit;

 -- Pokus o zaznamenani teroristicke organizace bez provedeneho obchodu
INSERT INTO ZAKAZNIK VALUES ('Zakaznik_bez_obchodu', default, 'placeholder');
INSERT INTO TERORISTICKA_ORGANIZACE VALUES (1, 'Zakaznik_bez_obchodu', 1, 'placeholder');

commit;


 -- Pokus o zaznamenani teroristicke organizace s provedenym obchodem
INSERT INTO ZAKAZNIK VALUES ('Zakaznik_s_obchodem', default, 'placeholder');
INSERT INTO CLEN VALUES (default, null, null, 'clen', 1, '?');
INSERT INTO OBCHODNIK VALUES (1, 'alias', 1);
INSERT INTO OBCHOD VALUES (default, 1, 2,'1.1.2000');
INSERT INTO TERORISTICKA_ORGANIZACE VALUES (2, 'Zakaznik_s_obchodem', 1, 'placeholder');

commit;