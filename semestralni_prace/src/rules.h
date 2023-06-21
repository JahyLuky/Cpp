#pragma once
#include <iostream>
#include "board.h"
#include "player.h"
#include "move.h"

/**
 * Class that takes care of chess rules
 */
class Rules {
public:
    /**
     * @brief Default constructor
     */
    Rules() = default;
    /**
     * @brief Checks if king is in check
     * @param board Chess board
     * @param pos King's position
     * @param color King's color
     * @return TRUE -> king is checked, FALSE -> otherwise
     */
    bool checked(const Board &board, const Position &pos, char color);
    /**
     * @brief Checks if king is in checkmate
     * @param board Chess board
     * @param player PLayer
     * @return TRUE -> checkmate, FALSE -> no checkmate
     */
    bool checkmate(Board &board, Player &player);
    /**
     * @brief Check if stalemate occurs
     * @param board Chess board
     * @param player Player
     * @return TRUE -> stalemate, FALSE -> no stalemate
     */
    bool stalemate(Board &board, Player &player);
};