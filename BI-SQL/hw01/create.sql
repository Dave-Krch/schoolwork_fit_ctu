BEGIN

  FOR i IN (SELECT us.sequence_name
              FROM USER_SEQUENCES us) LOOP
    EXECUTE IMMEDIATE 'drop sequence '|| i.sequence_name ||'';
  END LOOP;

  FOR i IN (SELECT ut.table_name
              FROM USER_TABLES ut) LOOP
    EXECUTE IMMEDIATE 'drop table '|| i.table_name ||' CASCADE CONSTRAINTS ';
  END LOOP;

END;

/*
CREATE SEQUENCE blizka_osoba_seq START WITH 1 INCREMENT BY 1;

CREATE TABLE blizka_osoba (
    id_blizka_osoba INTEGER DEFAULT blizka_osoba_seq.nextval PRIMARY KEY NOT NULL,
    id_bydliste INTEGER NOT NULL,
    jmeno VARCHAR(64) NOT NULL,
    vek INTEGER NOT NULL,
    pohlavi VARCHAR(16)
);

 */
-- ------------------------------------------------------------------------------------
CREATE SEQUENCE bydliste_seq START WITH 1 INCREMENT BY 1;

CREATE TABLE bydliste (
    id_bydliste INTEGER DEFAULT bydliste_seq.nextval PRIMARY KEY NOT NULL,
    stat VARCHAR(64) NOT NULL,
    mesto VARCHAR(64) NOT NULL,
    ulice VARCHAR(64) NOT NULL,
    cislo_domu INTEGER NOT NULL
);
-- ------------------------------------------------------------------------------------

CREATE SEQUENCE clen_seq START WITH 1 INCREMENT BY 1;

CREATE TABLE clen (
    id_clen NUMBER DEFAULT clen_seq.nextval PRIMARY KEY NOT NULL,
    id_bydliste INTEGER NOT NULL,
    obchodnik_id_clen INTEGER,
    jmeno VARCHAR(64) NOT NULL,
    vek INTEGER NOT NULL,
    pohlavi VARCHAR(16)
);

-- ------------------------------------------------------------------------------------
/*
CREATE TABLE drogovy_baron (
    id_zakaznik INTEGER NOT NULL PRIMARY KEY ,
    --jmeno VARCHAR(64) NOT NULL,
    oblibena_droga VARCHAR(64) NOT NULL,
    pocet_manzelek INTEGER
);

 */
-- ------------------------------------------------------------------------------------
/*
CREATE SEQUENCE jazyk_seq START WITH 1 INCREMENT BY 1;

CREATE TABLE jazyk (
    id_jazyk INTEGER DEFAULT jazyk_seq.nextval PRIMARY KEY NOT NULL,
    jmeno VARCHAR(64) NOT NULL
);

 */
-- ------------------------------------------------------------------------------------

CREATE SEQUENCE obchod_seq START WITH 1 INCREMENT BY 1;

CREATE TABLE obchod (
    id_obchod INTEGER DEFAULT obchod_seq.nextval NOT NULL,
    id_clen INTEGER NOT NULL,
    id_zakaznik INTEGER NOT NULL,
    datum DATE NOT NULL
);
ALTER TABLE obchod ADD CONSTRAINT pk_obchod PRIMARY KEY (id_obchod, id_clen, id_zakaznik);

-- ------------------------------------------------------------------------------------
CREATE TABLE obchodnik (
    id_clen INTEGER NOT NULL PRIMARY KEY,
    alias VARCHAR(64) NOT NULL UNIQUE,
    bodycount INTEGER NOT NULL
);

-- ------------------------------------------------------------------------------------
/*
CREATE TABLE obsah_obchodu (
    id_obchod INTEGER NOT NULL  ,
    id_clen INTEGER NOT NULL  ,
    id_zakaznik INTEGER NOT NULL ,
    typ VARCHAR(64) NOT NULL,
    cena INTEGER NOT NULL
);
ALTER TABLE obsah_obchodu ADD CONSTRAINT pk_obsah_obchodu PRIMARY KEY (id_obchod, id_clen, id_zakaznik);


 */
-- ------------------------------------------------------------------------------------
/*
CREATE TABLE teroristicka_organizace (
    id_zakaznik INTEGER NOT NULL PRIMARY KEY ,
    jmeno_vudce VARCHAR(64) NOT NULL,
    pocet_clenu INTEGER NOT NULL,
    uhlavni_nepritel VARCHAR(64) NOT NULL
);

 */
-- ------------------------------------------------------------------------------------
/*
CREATE TABLE vztah (
    id_clen INTEGER NOT NULL,
    id_blizka_osoba INTEGER NOT NULL,
    druh_vztahu VARCHAR(64) NOT NULL
);
ALTER TABLE vztah ADD CONSTRAINT pk_vztah PRIMARY KEY (id_clen, id_blizka_osoba);


 */
-- ------------------------------------------------------------------------------------

CREATE SEQUENCE zakaznik_seq START WITH 1 INCREMENT BY 1;

CREATE TABLE zakaznik (
    jmeno VARCHAR(64) NOT NULL,
    id_zakaznik INTEGER DEFAULT zakaznik_seq.nextval PRIMARY KEY NOT NULL,
    zeme_pusobeni VARCHAR(64) NOT NULL
);

-- ------------------------------------------------------------------------------------
/*
CREATE TABLE obchodnik_jazyk (
    id_clen INTEGER NOT NULL,
    id_jazyk INTEGER NOT NULL
);
ALTER TABLE obchodnik_jazyk ADD CONSTRAINT pk_obchodnik_jazyk PRIMARY KEY (id_clen, id_jazyk);

 */
-- ------------------------------------------------------------------------------------
/*
CREATE TABLE zakaznik_jazyk (
    id_zakaznik INTEGER NOT NULL,
    id_jazyk INTEGER NOT NULL
);
ALTER TABLE zakaznik_jazyk ADD CONSTRAINT pk_zakaznik_jazyk PRIMARY KEY (id_zakaznik, id_jazyk);


 */
-- vedlejsi klice a stuff

-- ALTER TABLE blizka_osoba ADD CONSTRAINT fk_blizka_osoba_bydliste FOREIGN KEY (id_bydliste) REFERENCES bydliste (id_bydliste) ON DELETE CASCADE;

ALTER TABLE clen ADD CONSTRAINT fk_clen_bydliste FOREIGN KEY (id_bydliste) REFERENCES bydliste (id_bydliste) ON DELETE CASCADE;
ALTER TABLE clen ADD CONSTRAINT fk_clen_obchodnik FOREIGN KEY (obchodnik_id_clen) REFERENCES obchodnik (id_clen) ON DELETE CASCADE;

-- ALTER TABLE drogovy_baron ADD CONSTRAINT fk_drogovy_baron_zakaznik FOREIGN KEY (id_zakaznik) REFERENCES zakaznik (id_zakaznik) ON DELETE CASCADE;

ALTER TABLE obchod ADD CONSTRAINT fk_obchod_obchodnik FOREIGN KEY (id_clen) REFERENCES obchodnik (id_clen) ON DELETE CASCADE;
ALTER TABLE obchod ADD CONSTRAINT fk_obchod_zakaznik FOREIGN KEY (id_zakaznik) REFERENCES zakaznik (id_zakaznik) ON DELETE CASCADE;

ALTER TABLE obchodnik ADD CONSTRAINT fk_obchodnik_clen FOREIGN KEY (id_clen) REFERENCES clen (id_clen) ON DELETE CASCADE;

-- ALTER TABLE obsah_obchodu ADD CONSTRAINT fk_obsah_obchodu_obchod FOREIGN KEY (id_obchod, id_clen, id_zakaznik) REFERENCES obchod (id_obchod, id_clen, id_zakaznik) ON DELETE CASCADE;

-- ALTER TABLE teroristicka_organizace ADD CONSTRAINT fk_ter_org_zaka FOREIGN KEY (id_zakaznik) REFERENCES zakaznik (id_zakaznik) ON DELETE CASCADE;

--ALTER TABLE vztah ADD CONSTRAINT fk_vztah_clen FOREIGN KEY (id_clen) REFERENCES clen (id_clen) ON DELETE CASCADE;
--ALTER TABLE vztah ADD CONSTRAINT fk_vztah_blizka_osoba FOREIGN KEY (id_blizka_osoba) REFERENCES blizka_osoba (id_blizka_osoba) ON DELETE CASCADE;

--ALTER TABLE obchodnik_jazyk ADD CONSTRAINT fk_obch_jazyk_obchodnik FOREIGN KEY (id_clen) REFERENCES obchodnik (id_clen) ON DELETE CASCADE;
--ALTER TABLE obchodnik_jazyk ADD CONSTRAINT fk_obch_jazyk_jazyk FOREIGN KEY (id_jazyk) REFERENCES jazyk (id_jazyk) ON DELETE CASCADE;

--ALTER TABLE zakaznik_jazyk ADD CONSTRAINT fk_zakaznik_jazyk_zakaznik FOREIGN KEY (id_zakaznik) REFERENCES zakaznik (id_zakaznik) ON DELETE CASCADE;
--ALTER TABLE zakaznik_jazyk ADD CONSTRAINT fk_zakaznik_jazyk_jazyk FOREIGN KEY (id_jazyk) REFERENCES jazyk (id_jazyk) ON DELETE CASCADE;

CREATE TABLE jmena (
    jmeno VARCHAR(64)
);

CREATE TABLE zeme (
    zeme VARCHAR(64)
);