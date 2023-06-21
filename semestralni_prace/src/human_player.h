#pragma once
#include "player.h"

/**
 * @brief Represents human player
 */
class HumanPlayer : public Player {
public:
    /**
     * @brief Default Constructor
     */
    HumanPlayer() = default;
    /**
     * @brief Constructor
     * @param color Player's color
     */
    HumanPlayer(char white_plays);
    /**
     * @brief Checks player's pawn promotion
     * @param piece Chess piece
     * @return TRUE -> correct promotion, FALSE -> incorrect promotion
     */
    bool get_promotion(char &piece) override;
    /**
     * @brief Gets player's move
     * @param board chess board
     * @return TRUE -> valid move, FALSE -> invalid move
     */
    bool get_move(Board&board) override;
};