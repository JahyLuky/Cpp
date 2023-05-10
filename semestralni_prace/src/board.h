#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "piece.h"

// Size of the chess board
const int BOARD_SIZE = 8;

/**
 * Represents square of chess board
 */
struct Square {
    Position pos_;
    // chess piece or empty square (nullptr)
    std::unique_ptr<Piece> piece_;

    Square(Position pos, std::unique_ptr<Piece> piece)
            : pos_(pos), piece_(std::move(piece)) {}

};

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
    std::vector<Square> squares_;
};