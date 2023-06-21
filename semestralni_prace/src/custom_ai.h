#pragma once
#include "player.h"
#include "move.h"

/**
 * @brief Represents Custom AI
 */
class Custom_AI : public Player {
public:
    /**
     * @brief Default Constructor
     */
    Custom_AI() = default;
    /**
     * @brief Constructor
     * @param color Player's color
     */
    Custom_AI(char color);
    /**
     * @brief Checks player's pawn promotion
     * @param piece Chess piece
     * @return TRUE -> correct promotion, FALSE -> incorrect promotion
     */
    bool get_promotion(char &piece) override;
    /**
     *
     * @param pieceType Piece's type
     * @return Piece's priority
     */
    int piece_priority(const char pieceType) const;
    /**
     * @brief Counts all moves for all pieces
     * @param board Chess board
     */
    void possible_pieces_and_moves(Board &board);
    /**
     * @brief Gets player's move
     * @param board Chess board
     * @return TRUE -> valid move, FALSE -> invalid move
     */
    bool get_move(Board &board) override;
};