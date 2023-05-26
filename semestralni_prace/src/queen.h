#pragma once

#include "piece.h"

/**
 * Represents QUEEN
 */
class Queen : public Piece {
public:
    Queen(char name, char color, const Position &pos)
            : Piece(name, color, pos) {}

    Queen *clone() const override;

    std::vector<Position> *possible_moves() override;
};