#pragma once

#include "piece.h"

/**
 * Represents ROOK
 */
class Rook : public Piece {
public:
    Rook(char name, char color, const Position &pos)
            : Piece(name, color, pos) {}

    Rook *clone() const override;

    std::vector<Position>* possible_moves() override;
};