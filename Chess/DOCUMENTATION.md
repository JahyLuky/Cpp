# Chess game
Autor: Lukáš Jahelka

---

## Installation

To install my chess game, follow these steps:

1. Clone the repository.
2. Run `make` to compile the game.
3. Run `./chess` to run the game.

## Play

To play my chess game, run `./chess` and follow the on-screen prompts.

---

## Popis

Dostanete na výběr 3 možnosti:

* `Tutorial` - interaktivní tutoriál
* `Hra proti dalšímu hráči` - klasická hra šachů pro 2 hráče
* `Hra proti umělé inteligenci` - hra proti umělé inteligenci
    1. minimax
    2. custom AI
    3. custom AI2

```
Chess game
Choose how you want to play.

------MENU------
Choose your type of game:
(t):    Tutorial
(h):    Player vs Player
(a):    Player vs AI
(l):    Load game
(e):    End of program
$h

Player vs Player
Commands:
(draw) for draw
(resign) for resign
(save) for saving your current game

Game starts!

   a  b  c  d  e  f  g  h
  ________________________
8| r  n  b  q  k  b  n  r |8
7| p  p  p  p  p  p  p  p |7
6|                        |6
5|                        |5
4|                        |4
3|                        |3
2| P  P  P  P  P  P  P  P |2
1| R  N  B  Q  K  B  N  R |1
  ------------------------
   a  b  c  d  e  f  g  h

WHITE plays!
What is your move?
$e2 e4
   a  b  c  d  e  f  g  h
  ________________________
8| R  N  B  Q  K  B  N  R |8
7| p  p  p  p  p  p  p  p |7
6|                        |6
5|                        |5
4|             P          |4
3|                        |3
2| P  P  P  P     P  P  P |2
1| R  N  B  Q  K  B  N  R |1
  ------------------------
   a  b  c  d  e  f  g  h

BLACK plays!
What is your move?
$...
```

Bílé figurky jsou psány malými písmeny ('P'), černé figurky jsou psány velkými písmeny ('p').

Pohyb se zadává ve formátu: `startovní_pozice` `cílová_pozice` ("e2 e4").

Jakmile hráč zadá korektní pozice, hra pohne figurkou, vypíše aktuální šachovnici a předá tah oponentovi.
V případě, že hráč zadá špatné pozice, bude vyzván k opětovnému zadání pozic.

Pokud hráč zadá místo pohybu figurky: `save`, následně bude uživatel vyzván, aby zadal jméno souboru, např.: `game1.txt`, program uloží hru jako `game1.txt` do adresáře `examples/`.

Po skončení hry se vypíše výsledek hry: `WHITE WINS`, `BLACK WINS`, `TIE`, `PAT`.
