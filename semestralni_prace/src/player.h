#pragma once

#include <iostream>

#include "board.h"
#include "piece.h"

/**
 * Abstract class for player
 */
class Player {
public:
    Player() = default;

    Player(char color);

    int coordinates_to_int(char c);

    int check_col(char c);

    /**
     * @param start
     * @param end
     * @param old_pos -> convert and saves start position
     * @param new_pos -> convert and saves end position
     * @return true -> saves correct position
     * @return false -> wrong position
     */
    bool extract_input(const std::string &start, const std::string &end, Position &old_pos, Position &new_pos);

    /**
     * @param chess board
     * @return true -> valid moves in start_ and end_
     * @return false -> no valid moves
     */
    virtual bool get_move(Board &board) = 0;

    virtual Player *clone() const = 0;

    char color_;
    Position start_;
    Position end_;
    std::vector<char> captures_;
protected:
};