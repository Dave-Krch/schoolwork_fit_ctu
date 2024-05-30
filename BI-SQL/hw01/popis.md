# Popis řešení

  - Pro tvoreni nahodnych hodnot jsem nejdrive pridal tabulky "jmena" a "zeme",
  ze kterych se v procedurach nahodne vybira
  - Pomocne tabulky a placeholder radek v tabulce "bydliste" vyplni inser_default_values.sql
  - Poznamka k jazyku: DBS semsetralku jsem psal 2 roky zpet a od te doby jsem zacal programovat v anglictine, doufam to nebude zpusobovat moc velky zmatek.
Veskery kod krome nazvu tabulek a sloupcu by uz mel mit jednotny jazyk

## procedura 1

- nad tabulkou: zakaznik
- nazev procedury: fill_zakaznik 

### Popis:
Vytvori se tabulky jmen a zemi, pote se provede p_count iteraci cyklu, 
kde se v kazde iteraci vyberou nahodne hodnoty a ty se vlozi do tabulky zakaznik

## procedura 2 

- nad tabulkou: obchodnik a clen (obchodnik je role clena, pro kazdeho obchodnika se tedy musi pridat i clen)
- nazev procedury: fill_obchodnik

### Popis:
Podobne jako u predchozi procedury. Navic se generuji nahodna cisla vek a bodycount.

Sloupec alias v tabulce obchodnik je unique, ve vnorenem cyklu se proto generuje alias dokud nebude nalezen unikatni, nebo dokud se nezkus vsechny mozna jmena.
Pokud jsou vypotrebovana jmena, nic se neprida.

## procedura 3 

- nad tabulkou: obchod
- nazev procedury: fill_obchod

### Popis:
Nejdrive se vyplni tabulka vsech moznych kombinaci id z tabulek obchodnik a zakaznik.

V cyklu se pote vzdy vybere nahodna kombinace, vygeneruje se nahodne datum a vlozi se do tabulky. Obchody nemusi byt unique a proto se neprovadi zadna kontrola.


