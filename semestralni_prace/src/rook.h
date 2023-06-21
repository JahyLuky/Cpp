#pragma once
#include "piece.h"

/**
 * Represents ROOK
 */
class Rook : public Piece {
public:
    /**
     * @brief Constructor
     * @param name
     * @param color
     * @param pos
     */
    Rook(char name, char color, const Position &pos);
    /**
     * @brief Clone function
     * @return return cloned bishop
     */
    Rook *clone() const override;
    /**
     * @brief Find all possible move based on piece type
     * @return vector of possible moves
     */
    std::vector<Position>* possible_moves() override;
};