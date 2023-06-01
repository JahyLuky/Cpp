#pragma once

#include <iostream>
#include <vector>

#include "square.h"
#include "position.h"

#include "pawn.h"
#include "queen.h"
#include "king.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"

// Size of the chess board
const int BOARD_SIZE = 8;

/**
 * Represents classical chess board
 *
 */
class Board {
public:
    Board() = default;

    // To indicate end of game
    bool game_over = false;

    /**
     * Initialize classical chess board
     * @return Board with pieces
     */
    void init_board();

    void print_color_board() const;

    void print_basic_board() const;

    // Stores each square of chess board
    std::vector<std::vector<Square>> squares_;
    Square start_;
    Square end_;
};
