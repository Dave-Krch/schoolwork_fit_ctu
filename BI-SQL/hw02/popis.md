# Popis řešení

## Integritní omezení 1

- Zakaznik nemuze by v databazi ulozeny jako teroristicka organizace, pokud neprovedl zadny obchod, podle ktereho by se dalo urcit, jaky typ zakaznika je
- Stejne IO by ve finalni podobe databaze bylo i pro tabulku drogovy_baron, implementace bude temer stejna

### Způsob řešeni 

-  Trigger nad tabulkou teroristicka_organizace, ktery zabrani insertu, pokud neexistuje potrebny obchod

## Integritní omezení 2

- Clen kartelu se muze stat obchodnikem, pouze pokud ma zaznamenane v databazi alespon 2 cleny rodiny

### Způsob řešeni 

- Package, ktera obali zakladni dml operace nad tabulkou obchodnik



