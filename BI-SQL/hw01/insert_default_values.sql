BEGIN
    EXECUTE IMMEDIATE 'DELETE FROM "JMENA"';
    EXECUTE IMMEDIATE 'DELETE FROM "ZEME"';
    EXECUTE IMMEDIATE 'DELETE FROM "BYDLISTE"';
END;

INSERT INTO JMENA values ('James');
INSERT INTO JMENA values ('Robert');
INSERT INTO JMENA values ('John');
INSERT INTO JMENA values ('Michael');
INSERT INTO JMENA values ('David');
INSERT INTO JMENA values ('William');
INSERT INTO JMENA values ('Richard');
INSERT INTO JMENA values ('Joseph');
INSERT INTO JMENA values ('Thomas');
INSERT INTO JMENA values ('Christopher');
INSERT INTO JMENA values ('Mary');
INSERT INTO JMENA values ('Patricia');
INSERT INTO JMENA values ('Jennifer');
INSERT INTO JMENA values ('Linda');
INSERT INTO JMENA values ('Elizabeth');
INSERT INTO JMENA values ('Barbara');
INSERT INTO JMENA values ('Susan');
INSERT INTO JMENA values ('Jessica');
INSERT INTO JMENA values ('Sarah');
INSERT INTO JMENA values ('Karen');

INSERT INTO ZEME values ('Cesko');
INSERT INTO ZEME values ('Slovensko');
INSERT INTO ZEME values ('Nemecko');
INSERT INTO ZEME values ('Anglicko');
INSERT INTO ZEME values ('Japonsko');
INSERT INTO ZEME values ('Korejsko');
INSERT INTO ZEME values ('Cinsko');
INSERT INTO ZEME values ('Americko');
INSERT INTO ZEME values ('Polsko');
INSERT INTO ZEME values ('Francouzsko');

insert into bydliste (id_bydliste, stat, mesto, ulice, cislo_domu) values (-1, 'placeholder', 'placeholder', 'placeholder', 0);
