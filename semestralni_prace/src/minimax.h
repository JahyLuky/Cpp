#pragma once
#include "player.h"
#include "move.h"

/**
 * @brief Represents MINIMAX
 */
class Minimax : public Player {
public:
    /**
     * @brief Default Constructor
     */
    Minimax() = default;
    /**
     * @brief Constructor
     * @param color Player's color
     */
    Minimax(char color);
    /**
     * @brief Checks player's pawn promotion
     * @param piece Chess piece
     * @return TRUE -> correct promotion, FALSE -> incorrect promotion
     */
    bool get_promotion(char &piece) override;
    /**
     * @brief Gets piece's value
     * @param pieceType Chess piece's type
     * @return Piece's value
     */
    int get_piece_value(const char pieceType);
    /**
     * @brief Evaluates chess board
     * @param board Chess board
     * @return Number of evaluated chess board
     */
    int evaluate_board(const Board &board);
    /**
     * @brief Minimax algorithm
     * @param board Chess board
     * @param depth Depth
     * @param maximizingPlayer Maximize/minimize player's move
     * @param move Handles move
     * @return Best score
     */
    int minimax_algorithm(Board &board, int depth, bool maximizingPlayer, Move &move);
    /**
     * @brief Gets player's move
     * @param board chess board
     * @return TRUE -> valid move, FALSE -> invalid move
     */
    bool get_move(Board&board ) override;
    /**
     * @brief Stores depth
     */
    int depth_;
};