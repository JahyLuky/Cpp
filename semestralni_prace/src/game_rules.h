#pragma once
#include <iostream>
#include <vector>
#include "board.h"
#include "player.h"

/**
 * @brief Represents game rules
 */
class GameRules {
public:
    /**
     * @brief Default Constructor
     */
    GameRules() = default;
    /**
     * Updates player's moves
     * @param board Chess board
     * @param white White player
     * @param black Black player
     * @param white_last_moves White's moves
     * @param black_last_moves Black's moves
     */
    void update_threefold(Board &board, Player &white, Player &black, std::vector<Position> &white_last_moves, std::vector<Position> &black_last_moves);
    /**
     * @brief Checks if the threefold repetition rule applies
     * @param white_last_moves White's moves
     * @param black_last_moves Black's moves
     * @return TRUE -> threefold, FALSE -> no threefold
     */
    bool threefold(const std::vector<Position> &white_last_moves, const std::vector<Position> &black_last_moves) const;
    /**
     * @brief Checks if the dead position applies
     * @param board Chess board
     * @param white White player
     * @param black Black player
     * @return
     */
    bool dead_position(Board &board, const Player &white, const Player &black);
    /**
     * @brief Checks if the half-move rules applies
     * @param board Chess board
     * @param white White player
     * @param black Black player
     */
    void half_moves(Board &board, Player &white, Player &black);
    /**
     * @brief Checks if the stalemate rule applies
     * @param board Chess board
     * @param white White player
     * @param black Black player
     * @return TRUE -> stalemate, FALSE -> no stalemate
     */
    bool stalemate(Board&board, Player&white, Player&black);
};