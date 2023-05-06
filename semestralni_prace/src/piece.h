#pragma once

#include <iostream>

#include "move.h"

/**
 * Represents position on chess board
 * (row_, col_)
 */
struct Position {
    int row_;
    int col_;

    Position() = default;

    Position(int a, int b)
            : row_(a), col_(b) {}

    const friend std::ostream &operator<<(std::ostream &out, const Position &pos) {
        out << "row: " << pos.row_ << ", column: " << pos.col_;
        return out;
    }
};

/**
 * Abstract class for chess pieces
 */
class Piece {
public:
    Piece(char name, char color, const Position &pos)
            : name_(name), color_(color), pos_(pos) {}

    virtual char get_piece() const {
        return name_;
    }

    virtual void set_piece(char name) {
        name_ = name;
    }

    virtual void print(std::ostream &out, const Piece &item) const = 0;

    virtual char get_color() const {
        return color_;
    }

    virtual Position get_coor() const {
        return pos_;
    }

    virtual void set_coor(Position &other) {
        pos_.row_ = other.row_;
        pos_.col_ = other.col_;
    }

    virtual bool get_square() const {
        return is_on_black_square;
    }

    virtual void set_square(bool square) {
        is_on_black_square = square;
    }

    virtual Piece *clone() const = 0;

    //virtual void make_move() = 0;

    virtual ~Piece() = default;

    friend std::ostream &operator<<(std::ostream &out, const Piece &item);

protected:
    char name_;
    char color_;
    bool is_on_black_square;
    Position pos_;
};