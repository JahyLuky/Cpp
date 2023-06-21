#pragma once
#include <iostream>
#include <memory>

#include "piece.h"

/**
 * Represents square of chess board
 */
struct Square {
    /**
     * @brief Position
     */
    Position pos_;
    /**
     * @brief Piece
     */
    std::unique_ptr<Piece> piece_;
    /**
     * @brief Default constructor
     */
    Square() = default;
    /**
     * @brief Copy Position
     * @param pos
     */
    Square(const Position &pos);
    /**
     * @brief Copy Position and Piece
     * @param pos
     * @param piece
     */
    Square(const Position &pos, std::unique_ptr<Piece> piece);
    /**
     * @brief Copy Square
     * @param other
     */
    Square(const Square &other);
    /**
     * @brief Operator =
     * @param other
     * @return
     */
    Square &operator=(const Square &other);
    /**
     * @brief Destructor
     */
    ~Square() = default;
};