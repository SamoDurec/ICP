Nastroj na vizualnu editaciu, generovanie kodu a monitorovanie behu interpretovanych Petriho sieti
==================================================================================================

Autori
------
Samuel Durec (xdurecs00)
Daniela Pertova (xpertod00)

Popis nastroja
--------------
Aplikacia implementuje graficky editor a interpret casovanych event-driven Petriho sieti.

Nastroj umoznuje vytvarat, upravovat a simulovat Petriho siete pomocou grafickeho rozhrania
zalozeneho na Qt Graphics View frameworku.

Siet je mozne ukladat do textoveho formatu a znovu nacitavat.

Funkcionalita
-------------
Tento nastroj umoznuje:
  - nacitavanie sieti zo suboru
  - ukladanie sieti do suboru
  - graficku editaciu siete
      - pridavanie a mazanie miest, prechodov a hran
      - editaciu parametrov prvkov siete
  - simulaciu behu Petriho siete
  - monitorovanie aktualneho stavu siete
  - injektovanie vstupnych udalosti
  - podporu casovanych prechodov
  - logovanie priebehu simulacie
  - vyhodnocovanie straznych podmienok (valueof, atoi, tokens, defined)

Vacsina funkcii sa ovlada pomocou menu v lavom hornom rohu okna nastroja.
Pri mazani je potrebne kliknut na prvok urceny na zmazanie.
Injektovanie vstupnych udalosti sa vykonava pomocou panelu v dolnej casti okna.
Rezim editacie parametrov sa spusta dvojklikom na prvok.

Preklad a spustenie
-------------------
Preklad:
    make

Spustenie:
    make run

Dokumentacia
------------
Dokumentacia generovana nastrojom Doxygen:
    make doxygen

Vygenerovana dokumentacia bude dostupna v:
    doc/html/index.html

Konceptualny navrh (diagram tried a sekvencny diagram) je v subore:
    doc/diagrams.pdf

Dalsie prikazy
--------------
Vytvorenie archivu na odovzdanie:
    make pack

Vycistenie projektu:
    make clean

Zmazanie vygenerovanej dokumentacie:
    make clean-doxy

Potrebne zavislosti
-------------------
  - Qt5 alebo Qt6
  - g++
  - make
  - doxygen

Stav implementacie
------------------

KOMPLETNE implementovane:
  - Vizualny editor (miesta, prechody, hrany)
  - Nacitanie a ukladanie siete (.pn format)
  - Pridavanie/mazanie/editacia miest, prechodov, hran
  - Runtime - odpálenie prechodov, multiset semantika tokenov
  - Casovane prechody (timery, @delay)
  - Injektovanie vstupov za behu
  - Farebne zvyraznenie enabled (zlta) a pending timer (modra) prechodov
  - Log udalosti (odpálenia, timery, vstupy)
  - Vyhodnocovanie straznych podmienok:
      valueof("x"), atoi(), tokens("place"), defined("x")
  - Doxygen dokumentacia

CIASTOCNE implementovane (s obmedzeniami):
  - Strazne podmienky - podporovane su: ==, !=, >=, <=, >, 
    ale nie su podporovane zlozene vyrazy (&&, ||)
  - Vaha hrany je vzdy 1, nie je mozne ju editovat cez GUI

NEIMPLEMENTOVANE:
  - Akcie prechodov (do: { }) sa syntakticky ukladaju ale nevykonavaju
  - Place actions ({ output("out", 1); } pri miestach) sa nevykonavaju
  - Pozicie prvkov sa neukladaju do .pn suboru
  - output("out", x) funkcia
  - elapsed() funkcia
  - now() funkcia

Implementacne poznamky
----------------------
Graficka cast editora je implementovana pomocou Qt Graphics View frameworku.

Interpretacia Petriho siete je realizovana triedou NetRunner, ktora implementuje:
  - stabilizaciu siete
  - odpalenie prechodov
  - planovanie oneskrenych prechodov
  - spracovanie vstupnych udalosti
  - vyhodnocovanie straznych podmienok (valueof, atoi, tokens, defined)

Hrany su reprezentovane triedou ArcItem, ktora automaticky aktualizuje
svoju geometriu pri pohybe prepojenych objektov.