#pragma once
#include "player.h"
#include "move.h"

/**
 * @brief Represents Custom AI2
 */
class Custom_AI2 : public Player {
public:
    /**
     * @brief Default Constructor
     */
    Custom_AI2() = default;
    /**
     * @brief Constructor
     * @param color Player's color
     */
    Custom_AI2(char color);
    /**
     * @brief Checks player's pawn promotion
     * @param piece Chess piece
     * @return TRUE -> correct promotion, FALSE -> incorrect promotion
     */
    bool get_promotion(char &piece) override;
    /**
     * @brief Counts all moves for all pieces
     * @param board Chess board
     */
    void possible_pieces_and_moves(Board&board);
    /**
     * @brief Gets player's move
     * @param board Chess board
     * @return TRUE -> valid move, FALSE -> invalid move
     */
    bool get_move(Board&board ) override;
};