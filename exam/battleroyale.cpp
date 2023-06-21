/*
Je dána herní mapa, reprezentována jako mřížka o určité výšce a šířce. Po této mapě se můžete pohybovat po jednotlivých čtvercích (vždy pouze po jednom - buď nahoru, dolu, doprava, nebo doleva). Pohyb z jednoho čtverce na další trvá přesně 1 hodinu.

Úkolem je realizovat třídu CBattleRoyale, která má mít nasledující rozhraní:

třídní konstruktor(height, width, zones)

Má tři vstupní parametry. První dva určují výšku a šířku mapy, třetí parametr je seznam zakázaných zón.
destruktor

Uvolní použité prostředky(paměť)
findRoute(ySt, xSt, yEn, xEn)

Metoda bude mít za úkol najít v hracím poli nejkratší cestu ze startovní pozice do cílové pozice a vyhýbat se přitom zakázaným zónám. Metoda je dána čtveřicí vstupních parametrů(ySt a xSt, yEn, xEn). První dvojice vstupních parametrů ySt a xSt udává startovní pozici, ze které hledáme cestu. Na startovním poli vždy začínáme v čase 0. Druhou dvojicí vstupních parametrů je yEn a xEn, ta udává cílovou pozici, na kterou se chceme dostat.
Každá zóna je dána vždy trojicí atributů (yPos, xPos, h), kdy h je čas od kterého je zóna aktivní(zakázaná), a dvojice yPos a xPos udávají pozici zóny na mapě.

Při pohybu po mapě se nesmíte pohybovat přes pole, na kterých je aktivní zóna. Funkce vrací celkový čas přesunu ze startu do cíle, nebo std::logic_error v případě že žádná cesta neexistuje.

Metoda findRoute musí vyhodit výjimku std::logic_error pokud cesta ze startu do cíle nelze najít, nebo pokud se pozice nachází mimo mapu. Pozice musí být vždy menší než height-1 a width-1.

(Zadání píšu jen zpaměti, tak zadání a šablonu kdyžtak opravte, pokud je něco špatně, nebo pokud něco chybí… Případně piřihoďte i nějaký asserty, pokud si pamatujete.)
 */

#ifndef __PROGTEST__
#include <algorithm>
#include <assert.h>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class CPos {
public:
    CPos(int y, int x, int hour)
            : m_hour(hour), m_posY(y), m_posX(x) {
    }

    int m_hour;
    int m_posY;
    int m_posX;
};

#endif /* __PROGTEST__ */

class CBattleRoyale {
public:
    CBattleRoyale(unsigned height, unsigned width, const std::list<CPos> &zones) {
        // todo
    }

    // des (if needed)

    unsigned findRoute(int ySt, int xSt, int yEn, int xEn) const {
        // todo
    }

private:
    // todo
};

#ifndef __PROGTEST__

int main() {
    CBattleRoyale r1(5, 5, {});
    assert(r1.findRoute(0, 0, 4, 0) == 4);
    assert(r1.findRoute(4, 4, 4, 4) == 0);

    CBattleRoyale r2(6, 7, {CPos(1, 0, 1), CPos(2, 1, 2), CPos(3, 2, 5)});
    assert(r2.findRoute(0, 0, 4, 0) == 10);


    CBattleRoyale r3(8, 8, {CPos(0, 2, 1), CPos(3, 1, 2), CPos(2, 1, 0)});
    try {
        r3.findRoute(2, 1, 4, 7);
        assert("Exception missing!" == nullptr);
    } catch (const logic_error &e) {
    } catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }
    assert(r3.findRoute(0, 2, 3, 0) == 5);

    CBattleRoyale b(5, 5, {CPos(0, 1, 1), CPos(1, 1, 0)});
    assert(b.findRoute(0, 0, 2, 2) == 4);
    assert(b.findRoute(0, 0, 0, 2) == 6);
    assert(b.findRoute(3, 3, 3, 3) == 0);
    try {
        assert(b.findRoute(1, 1, 2, 1) == 1);
        assert("Kde vyjimka?" == nullptr);
    } catch (logic_error &x) {}
    try {
        assert(b.findRoute(1, 1, 1, 1) == 0);
        assert("Kde vyjimka? xd" == nullptr);
    } catch (logic_error &x) {}

    CBattleRoyale b1(5, 5, {CPos(2, 0, 2), CPos(2, 1, 1), CPos(2, 2, 1), CPos(2, 3, 3), CPos(2, 4, 4)});
    try {
        b1.findRoute(1, 1, 3, 1);
        assert("Kde vyjimka?" == nullptr);
    } catch (logic_error &x) {}

}

#endif /* __PROGTEST__ */