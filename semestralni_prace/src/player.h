#pragma once

#include "board.h"
#include "pawn.h"

class Player {
public:
    Player() = default;

    static void make_move(Board &src, const Position &old_pos, const Position &new_pos);

private:
};