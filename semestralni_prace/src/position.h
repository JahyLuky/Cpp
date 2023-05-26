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

    Position(int a, int b);

    Position(const Position &other);

    Position &operator=(const Position &other);

    bool operator==(const Position &other) const;

    bool operator!=(const Position &other) const;

    ~Position() = default;
};
