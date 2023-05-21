#pragma once

#include <iostream>

#include "menu.h"
#include "board.h"
//#include "piece.h"
//#include "pawn.h"
//#include "queen.h"
//#include "king.h"
//#include "rook.h"
//#include "bishop.h"
//#include "knight.h"
#include "player.h"
#include "human_player.h"
#include "ai_player.h"

class Application {
public:
    bool run() const;

    bool tutorial() const;

    void make_move(Board &board, Player &player);

    bool game(char game_type) const;

private:

};