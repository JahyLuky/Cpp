#pragma once

#include <iostream>

/**
 * Represents position on chess board
 * (row, col)
 */
struct Position {
    int row_;
    int col_;

    Position() = default;

    Position(int a, int b)
            : row_(a), col_(b) {}

    Position(const Position &other)
            : row_(other.row_), col_(other.col_) {}

    Position& operator=(const Position &other) {
        if (this != &other) {
            row_ = other.row_;
            col_ = other.col_;
        }
        return *this;
    }

    ~Position() = default;
};
