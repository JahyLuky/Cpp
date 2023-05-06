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
    // chess piece or nullptr -> empty
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

    bool game_over = false;

    /**
     * Initialize classical chess board
     * @return Board with pieces
     */
    void init_board();

    /**
     * Print chess board
     * @param src : board to be printed
     */
    void print_color_board() const;

    void print_basic_board() const;

    std::vector<Square> &get_squares();

private:
    // Stores each square of chess board
    std::vector<Square> squares_;
};