-- primy rodic
SELECT rodic.jmeno
FROM VYDRA dite INNER JOIN VYDRA rodic ON (dite.OTEC = rodic.CV)
WHERE dite.JMENO = 'Merilin'

WITH
    Rodice(generace, cv, jmeno, otec, matka, pohlavi) AS (
        SELECT 0, cv, jmeno, otec, matka, pohlavi
        FROM vydra v1
        WHERE cv = 10
        UNION ALL
        SELECT r.generace + 1,  v2.cv, v2.jmeno, v2.otec, v2.matka, v2.pohlavi
        FROM vydra v2, Rodice r
        WHERE r.otec = v2.CV or r.matka = v2.CV
        )
SELECT * FROM Rodice

WITH
    Rodice(generace, cv, jmeno, otec, matka, pohlavi) AS (
        SELECT 0, cv, jmeno, otec, matka, pohlavi
        FROM vydra v1
        WHERE cv = 10
        UNION ALL
        SELECT r.generace + 1,  v2.cv, v2.jmeno, v2.otec, v2.matka, v2.pohlavi
        FROM vydra v2, Rodice r
        WHERE r.otec = v2.CV
        )
SELECT * FROM Rodice

WITH
    Rodice(generace, cv, jmeno, otec, matka, pohlavi) AS (
        SELECT 0, cv, jmeno, otec, matka, pohlavi
        FROM vydra v1
        WHERE cv = 10
        UNION ALL
        SELECT r.generace + 1,  v2.cv, v2.jmeno, v2.otec, v2.matka, v2.pohlavi
        FROM vydra v2, Rodice r
        WHERE r.matka = v2.CV
        )
SELECT * FROM Rodice