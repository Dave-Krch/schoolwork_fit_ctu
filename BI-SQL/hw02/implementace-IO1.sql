create or replace TRIGGER ter_org_existuje_obchod
BEFORE INSERT on TERORISTICKA_ORGANIZACE FOR EACH ROW
DECLARE
    t_count int;
BEGIN
    SELECT count(*)
    INTO t_count
    FROM OBCHOD o
    WHERE o.ID_ZAKAZNIK = :NEW.ID_ZAKAZNIK;

    if t_count = 0 THEN
        RAISE_APPLICATION_ERROR(-20502,
          'Zakaznik nema zaznamenany obchod');
    end if;
END;