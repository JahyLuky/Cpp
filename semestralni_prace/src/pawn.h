#pragma once

#include "piece.h"

/**
 * Represents PAWN
 */
class Pawn : public Piece {
public:
    Pawn(char name, char color, const Position &pos)
            : Piece(name, color, pos) {}

    Pawn *clone() const override;

    std::vector<Position> *possible_moves() override;
};