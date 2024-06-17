# Zpracování dat

* pomocí PLPg/SQL

## Cíle
* Navrhnout, implementovat a otestovat jednoduchý scénář zpracování dat.

## Zadání

* Vymyslete si nějaký scénář pro úpravu dat, ve kterém budou tyto kroky
    * načtení vstupních dat v CSV formátu (**použije se příkaz **COPY**)
    * zpracování vstupních dat pomocí procedur/funkcí v plpgsql
        * při zpracování využijte nějaká další data (tabulky) v postgreSQL
    * výsledky exportujte v nějaké vhodné podobě (JSON/XML/CSV/...)    
* Scénář implementujte
* Zdokumentujte

## Inspirace a další nápověda

* očekávám příklad zhruba v rozsahu, který jsem předváděl na [https://courses.fit.cvut.cz/BI-SQL/tutorials/10/index.html](předposledním prosemináři)
* jíný příklad (z mého života)
    * zamestnavatel po mě chce, abych každý měsíc vykazoval hodiny
    * je to formalita, ale trvá na tom, že výkaz musí vypadat tak, že
        * vykazuji hodinu a půl denně (malý úvazek),
        * nesmím ho vykazovat o víkendech,
        * dovolená musí sedět s tím, jak jí čerpám a vykazuji v jiném systému
    * na vstupu mám csv tabulku s dovolenou, kterou nahraji do postgresql
    * spustím proceduru, která má jako parametr časový interval (první a poslední den v měsíci)
    * procedura vyrobí tabulku výkazu ve formátu latex, kterou uložím do souboru
* pokud se vám nepodaří vymyslet si vlastní use-case, můžete se (volně) inspirovat tímto
* aby byl implementovaný scénář prakticky užitečný, je lepší ho realizovat pomocí psql
    * netrvám na tom, ale rád bych vás k tomu inspiroval
    * což tak trochu obnáší instalovat si postgresql lokálně
    * nebo se dá použít postgresql v dockeru, ale to je ještě o něco složitější, pokud jste to dosud nedělali

## Očekávané výstupy a testování

* popis scénáře
* dokumentace funkčního řešeni
* ukázková vstupní a výstupní data
* samotné řešení v plpgsql (jako samostatný soubor)

## bodování a deadline

- max 15 bodů
- tento příklad ohodnotím před nebo při zkušebním pohovoru, stačí ho tedy odevzdat dříve než půjdete na zkoušku
