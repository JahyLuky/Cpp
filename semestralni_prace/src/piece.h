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
    // TODO: constructor into cpp
    Piece(char name, char color, const Position &pos)
            : name_(name), color_(color), pos_(pos) {}

    char get_piece() const;

    void set_piece(char name);

    virtual void print(std::ostream &out, const Piece &item) const = 0;

    char get_color() const;

    void set_color(char color);

    Position get_coor() const;

    void set_coor(Position &other);

    bool get_square() const;

    void set_square(bool square);

    virtual Piece *clone() const = 0;

    // takes actual position on board (pos_)
    // gives all possible moves
    virtual std::vector<Position>* possible_moves() = 0;

    virtual ~Piece() = default;

    friend std::ostream &operator<<(std::ostream &out, const Piece &item);

protected:
    char name_;
    char color_;
    bool is_on_black_square;
    Position pos_;
    std::vector<Position> moves_;
};
