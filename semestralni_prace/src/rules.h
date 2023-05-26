#pragma once

#include <iostream>

#include "board.h"
#include "player.h"

class Rules {
public:
    Rules() = default;

    bool valid_position(const Position &other);

    bool is_empty(const Square& square) const;

    bool is_same_color(const Square&square, const char &color) const;

    bool find_possible_move(const Board &board, Player &player, const Square &start, const Square &end) const;

    bool validate_move(const Board &board, Player &player);

    bool checked(const Board &board, const Position &pos, char color);

private:
};