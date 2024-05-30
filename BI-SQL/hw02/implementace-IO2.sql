CREATE OR REPLACE PACKAGE "obchodnik_wrapper"
authid definer
is
    not_enough_relationships EXCEPTION;
    row_missing EXCEPTION;
    PRAGMA EXCEPTION_INIT ( not_enough_relationships, -20001 );
    PRAGMA EXCEPTION_INIT ( row_missing, -20002 );

    PROCEDURE insert_(id_ INTEGER, alias_ VARCHAR2, bodycount_ INTEGER);

    -- zbytecne pro IO, ale nutne pokud zakazeme DML operace
    PROCEDURE delete_(id_ INTEGER);
    PROCEDURE delete_(alias_ VARCHAR2);

    PROCEDURE update_(id_ INTEGER, alias_ VARCHAR2, bodycount_ INTEGER);
    PROCEDURE update_id(old_id INTEGER, new_id INTEGER);
    PROCEDURE update_all(old_id INTEGER, new_id INTEGER, alias_ VARCHAR2, bodycount_ INTEGER);

end "obchodnik_wrapper";

CREATE OR REPLACE PACKAGE BODY "obchodnik_wrapper" IS

    PROCEDURE insert_(id_ INTEGER, alias_ VARCHAR2, bodycount_ INTEGER) IS
        t_count INTEGER;
    BEGIN
        SELECT count(*)
        INTO t_count
        FROM VZTAH
        WHERE ID_CLEN = id_;

        if t_count < 2 THEN
            RAISE_APPLICATION_ERROR(-20001, 'Not enough relationships for insertion.');
        end if;

        INSERT INTO OBCHODNIK VALUES (id_, alias_, bodycount_);

    END insert_;


    PROCEDURE delete_(id_ INTEGER) IS
    BEGIN

        DELETE FROM OBCHODNIK WHERE ID_CLEN = id_;
    END delete_;


    PROCEDURE delete_(alias_ VARCHAR2) IS
    BEGIN
        DELETE FROM OBCHODNIK o WHERE o.ALIAS = alias_;
    END delete_;


    PROCEDURE update_(id_ INTEGER, alias_ VARCHAR2, bodycount_ INTEGER) IS
        obchodnik_count INTEGER;
    BEGIN
        SELECT count(*)
        INTO obchodnik_count
        FROM OBCHODNIK
        WHERE ID_CLEN = id_;

        if obchodnik_count = 0 then
            RAISE_APPLICATION_ERROR(-20002, 'Row with given id_ does not exist');
        end if;

        UPDATE OBCHODNIK SET ALIAS = alias_, BODYCOUNT = bodycount_ WHERE ID_CLEN = id_;
    END update_;


    PROCEDURE update_id(old_id INTEGER, new_id INTEGER) IS
        obchodnik_count INTEGER;
        t_count integer;
    BEGIN

        SELECT count(*)
        INTO obchodnik_count
        FROM OBCHODNIK
        WHERE ID_CLEN = old_id;

        if obchodnik_count = 0 then
            RAISE_APPLICATION_ERROR(-20002, 'Row with given old_id does not exist');
        end if;

        SELECT count(*)
        INTO t_count
        FROM VZTAH
        WHERE ID_CLEN = new_id;

        if t_count < 2 THEN
            RAISE_APPLICATION_ERROR(-20001, 'New id does not have relationships for update.');
        end if;

        UPDATE OBCHODNIK SET ID_CLEN = new_id WHERE ID_CLEN = old_id;

    END update_id;


    PROCEDURE update_all(old_id INTEGER, new_id INTEGER, alias_ VARCHAR2, bodycount_ INTEGER) IS
        obchodnik_count INTEGER;
        t_count integer;
    BEGIN

        SELECT count(*)
        INTO obchodnik_count
        FROM OBCHODNIK
        WHERE ID_CLEN = old_id;

        if obchodnik_count = 0 then
            RAISE_APPLICATION_ERROR(-20002, 'Row with given old_id does not exist');
        end if;

        SELECT count(*)
        INTO t_count
        FROM VZTAH
        WHERE ID_CLEN = new_id;

        if t_count < 2 THEN
            RAISE_APPLICATION_ERROR(-20001, 'New id does not have enough relationships for update.');
        end if;

        UPDATE OBCHODNIK SET ID_CLEN = new_id, ALIAS = alias_, BODYCOUNT = bodycount_ WHERE ID_CLEN = old_id;

    END update_all;

END "obchodnik_wrapper";
