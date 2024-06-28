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

## Description

You have three options to choose from:

* `Tutorial` - interactive tutorial
* `Player vs Player` - classic chess game for 2 players
* `Player vs AI` - game against artificial intelligence
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


White pieces are represented by lowercase letters ('p'), black pieces by uppercase letters ('P').

Moves are entered in the format: `starting_position` `target_position` ("e2 e4").

Once the player enters correct positions, the game makes the move, displays the current chessboard, and passes the turn to the opponent.
If incorrect positions are entered, the player will be prompted to re-enter the positions.

If the player enters `save` instead of a move, they will be prompted to enter a filename, e.g., `game1.txt`. The program saves the game as `game1.txt` in the `examples/` directory.

After the game ends, the result will be displayed: `WHITE WINS`, `BLACK WINS`, `TIE`, or `STALEMATE`.

