#pragma once

#include "piece.h"

/**
 * Represents KNIGHT
 */
class Knight : public Piece {
public:
    /**
     * @brief Constructor
     * @param name
     * @param color
     * @param pos
     */
    Knight(char name, char color, const Position &pos);
    /**
     * @brief Clone function
     * @return return cloned bishop
     */
    Knight *clone() const override;
    /**
     * @brief Find all possible move based on piece type
     * @return vector of possible moves
     */
    std::vector<Position>* possible_moves() override;
};