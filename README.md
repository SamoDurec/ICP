# Nástroj na vizuálnu editáciu, generovanie kódu a monitorovanie behu interpretovaných Petriho sietí

## Autori
Samuel Durec (xdurecs00)

Daniela Pertová (xpertod00)

## Popis nástroja
Aplikácia implementuje grafický editor a interpret časovaných event-driven Petriho sietí.

Nástroj umožňuje vytvárať, upravovať a simulovať Petriho siete pomocou grafického rozhrania založeného
na Qt Graphics View frameworku.

Sieť je možné ukladať do textového formátu a znovu načítavať.

## Funkcionalita
Tento nástroj umožňuje:
- načítavanie sietí zo súboru
- ukladanie sietí do súboru
- grafickú editáciu siete
    - pridávanie a mazanie miest, prechodov a hrán
    - editáciu parametrov prvkov siete
- simuláciu behu Petriho siete
- monitorovanie aktuálneho stavu siete
- injektovanie vstupných udalostí
- podporu časovaných prechodov
- logovanie priebehu simulácie

Väčšina funkcií sa ovláda pomocou menu v ľavom hornom rohu okna nástroja. Pri mazaní je potrebné kliknúť na prvok určený na zmazanie.
Injektovanie vstupných udalostí sa vykonáva pomocou panelu v dolnej časti okna.
Režim editácie parametrov sa spúšťa dvojklikom na prvok.

## Preklad a spustenie

### Preklad:
Nástroj sa zostaví volaním príkazu:

    make

### Spustenie:
Nástroj sa spustí príkazom:

    make run

## Dokumentácia
Dokumentácia generovaná nástrojom Doxygen:

    make doxygen

Vygenerovaná dokumentácia bude dostupná v:

    doc/html/index.html

## Ďalšie príkazy

### Vytvorenie archívu na odovzdanie
    make pack

### Vyčistenie projektu
    make clean

### Zmazanie vygenerovanej dokumentácie
    make clean-doxy

## Potrebné závislosti
Projekt vyžaduje:
- Qt5
- g++
- make
- doxygen
- graphviz

## Známe obmedzenia
tu sa doplnia dve alebo tri veci, ktoré napríklad nie sú úplne dotiahnuté alebo podobne
- nelze editovat váhu hrany

## Implementačné poznámky
Grafická časť editora je implementovaná pomocou Qt Graphics View frameworku.

Interpretácia Petriho siete je realizovaná triedou NetRunner, ktorá implementuje:
- stabilizáciu siete
- odpálenie prechodov
- plánovanie oneskorených prechodov
- spracovanie vstupných udalostí

Hrany sú reprezentované triedou ArcItem, ktorá automaticky aktualizuje svoju geometriu pri pohybe prepojených objektov.