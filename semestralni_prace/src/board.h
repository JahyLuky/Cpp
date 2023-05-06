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

            /*
    Square(const Square &other)
            : pos_(other.pos_), piece_(other.piece_->clone()) {}

    Square& operator=(const Square& other) {
        if (this != &other) {
            pos_ = other.pos_;
            piece_ = other.piece_ ? std::make_unique<other.piece_>(*other.piece_) : nullptr;
        }
        return *this;
    }


    ~Square() = default;
*/
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
    static Board init_board();

    /**
     * Print chess board
     * @param src : board to be printed
     */
    static void print_board(const Board &src);


    // Stores each square of chess board
    std::vector<Square> squares_;
private:
};