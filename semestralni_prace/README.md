# Chess game (zadání "Hra: Šachy")
Autor: Lukáš Jahelka

## Zadání

Vaším cílem má být implementovat klasickou hru Šachy. Není nutné implementovat jen ty známé šachy, ale případně se inspirovat nějakou jejich variantou jakou jsou asijské šachy. V tom případě je potřeba pravidla dostatečně dobře popsat v zadání. A zároveň je nutné za každé z pravidel mít vhodnou alternativu, aby nedošlo k přehnanému zjednodušení zadání.

Implementujte následující varianty:

* tutoriál, který naučí hráče jednotlivé prvky a pravidla hry,
* pro 2 hráče na jednom počítači,
* pro hru proti počítači.

Hra musí splňovat následující funkcionality:

* Dodržování všech pravidel vč. rošády, braní mimochodem, proměna pěsce
* Hra musí řádně oznamovat stav hry a případně její konec (šach, mat, pat) a výsledek (kdo vyhrál).
* Implementujte alespoň 3 druhy umělé inteligence (náhodné tahy nestačí).
  * Jeden druh musí být škálovatelná umělá inteligence pomocí algoritmu minimax. Škálování pak probíhá na úrovni hloubky prohledávacího stromu.
* Hra je konfigurovatelná ze souboru:
  * umístění uložených her na disku,
  * zahájení šachových partií pro umělou inteligenci,
  * texty, rozestavění (příp. očekávané tahy) k tutoriálu.


Kde lze využít polymorfismus? (doporučené)

* pohyby figurek: král, dáma, věž, jezdec, ...
* jednotlivá pravidla: tahy, rošáda, braní mimochodem, proměna (jejich výběr pak může být konfigurovatelný), ...
* pravidla různých variant: klasické šachy, žravé šachy, asijské šachy, ...
* ovládání hráčů: lokální hráč, umělá inteligence (různé druhy), síťový hráč
* uživatelské rozhraní: konzolové, ncurses, SDL, OpenGL (různé druhy), ...

## Specifikace

Budu implementovat klasickou variantu hry *šachy*.

Hráč si po zapnutí hry vybere jak bude pokračovat.
1) *$make*
2) *$./chess*

Dostane na výběr 3 možnosti:

* `Tutorial` - vypíše pravidla hry 
(staci toto pro tento bod?
*"tutoriál, který naučí hráče jednotlivé prvky a pravidla hry,"*) / případně ukázky pravidel, např.: jak vypadá rošáda (kdy může nastat a jak ji udělat)
* `Hra proti dalšímu hráči` - hra připraví šachovnici
* `Hra proti umělé inteligenci` - výběr umělé inteligence, následně se připraví šachovnice a hráč si vybere barvu figurek (zda bude hrát první nebo druhý)
  1. minimax
  2. *další typ*
  3. *další typ*

Po výběru začíná hra.

```
Chess game
Choose how you want to play.

------MENU------
Choose your type of game:
(t):    Tutorial
(h):    Game vs human
(a):    Game vs AI
$h

Game vs human


Game starts!

   a  b  c  d  e  f  g  h
  ________________________
8| R  N  B  Q  K  B  N  R |8
7| P  P  P  P  P  P  P  P |7
6|                        |6
5|                        |5
4|                        |4
3|                        |3
2| p  p  p  p  p  p  p  p |2
1| r  n  b  q  k  b  n  r |1
  ------------------------
   a  b  c  d  e  f  g  h

WHITE plays!
What is your move?
$e2 e4

W: 
B:
   a  b  c  d  e  f  g  h
  ________________________
8| R  N  B  Q  K  B  N  R |8
7| P  P  P  P  P  P  P  P |7
6|                        |6
5|                        |5
4|             p          |4
3|                        |3
2| p  p  p  p     p  p  p |2
1| r  n  b  q  k  b  n  r |1
  ------------------------
   a  b  c  d  e  f  g  h

BLACK plays!
What is your move?
```
Bílé figurky jsou psány malými písmeny ('p'), černé figurky jsou psány velkými písmeny ('P').

Pohyb se zadává ve formátu: `startovní_pozice` `cílová_pozice` ("e2 e4").

Jakmile hráč zadá korektní pozice, hra pohne figurkou, vypíše aktuální šachovnici a předá tah oponentovi. 
V případě, že hráč zadá špatné pozice, bude vyzván k opětovnému zadání pozic.

<u>Aktuální stav hry:</u>
<br>`W:` zde budou vypsány sebrané figurky černého,
<br>`B:` zde budou vypsány sebrané figurky bílého.

<u>Soubory:</u>
<br>Pokud hráč zadá místo pohybu figurky: `save game1`, uloží se aktuální hra jako `game1.txt` do adresáře `examples/`.
//todo format ulozeni

<u>Specifická pravidla a situace hry:</u>
<br>`rošáda:` hra zkontroluje, zda je rošáda možná a provede ji, případně ohlásí neúspěch.
* třídy `king` a `rook` budou mít proměnné, zda se pohnuly
* třída `king` bude mít proměnnou, která zkontroluje, zda není v šachu
* -> pokud žádná proměnná neplatí, lze udělat rošádu

<br>`proměna pěšce:` jakmile hráč pohne pěšcem na poslední řádek, bude vyzván, aby určil novou figurku, která se vymění za pěsce.

<br>`braní mimochodem:` pokud hráč zadá tah, který implikuje, že chce brát mimochodem, hra zkontroluje, zda byla splněna kritéria.
* třída `board` si bude ukládat poslední tah
* třída `pawn` bude mít proměnnou, která kontroluje, zda byl udělán 1. tah -> ztrácí právo na pohyb o 2 políčka vpřed
* třída `pawn` bude mít proměnnou, která kontroluje, zda se pohnul o 2 políčka před sebe
* -> pokud byl poslední tah protihráče pěšcem, který se pohnul o 2 políčka, lze ho vzít mimochodem (náš pěsec musí být na korektní pozici)

<br>`král v šachu:`  <b>zde popíšu, jak bude fungovat pohyb figurek</b>
* jakmile hráč zadá pozice, hra najde figurku, zjistí její typ (pěšec, věž atd.), se kterou chce hráč pohnout
* této figurce najde všechny povolené tahy (dle typu figurky):
  * pokud je v cestě naše figurka nebo je mimo šachovnici, tah nebude povolen
  * pokud je v cestě protihráčova figurka, tah je povolen (případné vzatí figurky se bude řešit v metodě "make_move" třídy `player`)
* jelikož může nastat situace: Mějme na "e1 bílého krále", na "e5 černého střelce" a na "e6 černou věž", po pohnutí "střelcem na g3" se bílý král dostane do šachu od 2 figurek zároveň
  * před zadáním tahu se vždy podívám zda můj král není v šachu, pokud ANO, upozorním hráče, jinak hra pokračuje normálně
  * zda je král v šachu zjistím tak, že zkontroluji diagonály, sloupec a řádek na kterém se nachází (výsledek bude uložen ve třídě `king`)
* v případě, že je král v šachu, se při hledání všech povolených tahů budou zvažovat pouze ty, který zabrání šachu
  * povolené tahy figurky se budou shodovat s tahy, přes které je král v šachu, včetně útočné figurky

Po skončení hry se vypíše výsledek hry: `WHITE WINS`, `BLACK WINS`, `TIE`.

## Polymorfismus

Třída `piece:`
* má za potomky jednotlivé šachové figurky
* má polymorfní metodu `possible_moves`, která na základě typu figurky spočítá její povolené tahy
  * tato metoda se volá v `HumanPlayer::make_move` a `AIPlayer::make_move`
* jednotlivé figurky využívají `piece` pro zjištění jejich pozice na šachovnici, barvy a hlavně jejich typu (zda je figurka král, pěšec atd.)

Třída `player:`
* má dva potomky `human_player` a `ai_player`
* má polymorfní metodu `make_move`, která si vyžadá: `aktuální šachovnice`, `startovní_pozice`, `cílová_pozice` a vrátí `true`, pokud je tah platný, v opačném případě vrátí `false`
  * tato metoda se volá v `application::game`
* implementace pro `human_player` očekává, že se dotážeme na další tah pomocí terminálu
* implementace pro `ai_player` vypočítá tah z aktuální šachovnice (podle zvolené AI se vybere metoda = AI algoritmus, která určí tah)


### Poznámka pro opravující
Hra je spustitelná a hratelná (v rámci možností).
Všechny figurky jsou aktuálně nastaveny jako `pawn` a mohou pouze o 1 nebo 2 políčka vpřed.
Ukázka polymorfismu je v `piece` -> `pawn` a `player` -> `human_player` (asi bude přesnější, než slovní popis).
Dále je kód ve třídách `application`, `board` a `menu`.