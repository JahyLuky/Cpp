#pragma once

#include "piece.h"

/**
 * Represents BISHOP
 */
class Bishop : public Piece {
public:
    Bishop(char name, char color, const Position &pos)
            : Piece(name, color, pos) {}

    Bishop *clone() const override;

    std::vector<Position>* possible_moves(Board & board) override;
};