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

Budeme implementovat klasickou variantu hry šachy. TODO: pravidla.

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
8  R  N  B  Q  K  B  N  R  8
7  P  P  P  P  P  P  P  P  7
6                          6
5                          5
4                          4
3                          3
2  p  p  p  p  p  p  p  p  2
1  r  n  b  q  k  b  n  r  1
   a  b  c  d  e  f  g  h
What is your move?
$e2 e4
   a  b  c  d  e  f  g  h
8  R  N  B  Q  K  B  N  R  8
7  P  P  P  P  P  P  P  P  7
6                          6
5                          5
4              p           4
3                          3
2  p  p  p  p     p  p  p  2
1  r  n  b  q  k  b  n  r  1
   a  b  c  d  e  f  g  h
What is your move?
```

TODO: pripadny dalsi popis

## Polymorfismus

TODO: popsat
