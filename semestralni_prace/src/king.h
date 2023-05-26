#pragma once

#include "piece.h"

/**
 * Represents KING
 */
class King : public Piece {
public:
    King(char name, char color, const Position &pos)
            : Piece(name, color, pos) {}

    King *clone() const override;

    std::vector<Position>* possible_moves() override;
};