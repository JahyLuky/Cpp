#pragma once

#include <iostream>
#include <vector>

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
};

/**
 * Abstract class for chess pieces
 */
class Piece {
public:
    Piece(char name, char color, const Position &pos);

    char get_piece() const;

    void set_piece(char name);

    char get_color() const;

    void set_color(char color);

    Position get_position() const;

    void set_position(const Position &other);

    virtual Piece *clone() const = 0;

    /** takes actual position on board (pos_)
     * gives all possible moves (based on piece's type)
     */
    virtual std::vector<Position> *possible_moves() = 0;

    virtual ~Piece() = default;

    // true -> can move 2 squares ahead
    // false -> otherwise
    bool first_move_ = true;
protected:
    char type_;
    char color_;
    Position pos_;
    std::vector<Position> moves_;
};