DECLARE
    vysledek FLOAT(63);
BEGIN
    vysledek := kolik_penez(3);
    DBMS_OUTPUT.PUT_LINE('Zamestnance s id 3 vydelal: ' || vysledek);

    vysledek := kolik_penez(1);
    DBMS_OUTPUT.PUT_LINE('Zamestnance s id 1 vydelal: ' || vysledek);

    vysledek := kolik_penez(17);
    DBMS_OUTPUT.PUT_LINE('Zamestnance s id 17 vydelal: ' || vysledek);

    vysledek := kolik_penez(33);
    DBMS_OUTPUT.PUT_LINE('Zamestnance s id 33 vydelal: ' || vysledek);
END;