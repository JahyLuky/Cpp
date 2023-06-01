#pragma once

#include <iostream>
#include <memory>

#include "piece.h"

/**
 * Represents square of chess board
 */
struct Square {
    Position pos_;
    // chess piece or empty square (nullptr)
    std::unique_ptr<Piece> piece_;

    Square() = default;

    Square(const Position &pos, std::unique_ptr<Piece> piece);

    Square(const Square &other);

    Square &operator=(const Square &other);

    ~Square() = default;
};