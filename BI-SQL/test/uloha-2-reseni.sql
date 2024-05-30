create or replace TRIGGER hlidej_zanr
BEFORE INSERT on FILM_ZANR FOR EACH ROW
DECLARE
    t_count int;
BEGIN
    SELECT count(*)
    INTO t_count
    FROM FILM_ZANR
    WHERE ID_FILMU = :NEW.ID_FILMU;

    if t_count > 3 THEN
        RAISE_APPLICATION_ERROR(-20002, 'Film ' || :NEW.ID_FILMU || ' nemůže být přiřazen do více než 4 žánrů');
    end if;
END;