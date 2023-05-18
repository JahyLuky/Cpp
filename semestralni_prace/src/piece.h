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

    Position get_coor() const;

    void set_coor(Position &other);

    virtual Piece *clone() const = 0;

    /** takes actual position on board (pos_)
     * gives all possible moves (based on piece's type)
     */
    virtual std::vector<Position> *possible_moves() = 0;

    virtual ~Piece() = default;

protected:
    char type_;
    char color_;
    Position pos_;
    std::vector<Position> moves_;
};
