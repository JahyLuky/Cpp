#pragma once

#include <iostream>
#include <vector>

#include "position.h"

class Board; // Forward declaration of the Board class

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

    bool valid_position(const Position &other) const;

    void add_move(const Position &dest);

    bool is_empty(const Board &board, const Position &dest) const;

    bool is_same_color(const Board &board, const Position &dest) const;

    virtual Piece *clone() const = 0;

    /**
     * gives all possible moves (based on piece's type)
     */
    virtual std::vector<Position> *possible_moves(Board &board) = 0;

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
