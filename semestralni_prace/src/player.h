#pragma once

#include <iostream>

#include "board.h"
#include "piece.h"
#include "pawn.h"

class Player {
public:
    Player() = default;

    virtual bool make_move(Board &src, const Position &old_pos, const Position &new_pos) = 0;

    virtual Player *clone() const = 0;

private:
};