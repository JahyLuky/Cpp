#pragma once

#include "piece.h"

/**
 * Represents KNIGHT
 */
class Knight : public Piece {
public:
    Knight(char name, char color, const Position &pos)
            : Piece(name, color, pos) {}

    Knight *clone() const override;

    std::vector<Position>* possible_moves(Board & board) override;
};