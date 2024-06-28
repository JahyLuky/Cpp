#pragma once
#include <iostream>
#include "board.h"
#include "player.h"

/**
 * @brief Represents MOVE
 */
class Move {
public:
    /**
     * @brief Moves piece from start to end
     * @param start
     * @param end
     */
    void move_piece(Square &start, Square &end);
    /**
     * @brief Handles casting
     * @param board Chess board
     * @param player Player
     * @param start Start position
     * @param end End position
     */
    void handle_castling(Board &board, Player &player, Square &start, Square &end);
    /**
     * @brief Handles pawn promotion
     * @param board Chess board
     * @param player Player
     * @param start Start position
     * @param end End position
     * @param piece Type of piece to promote
     */
    void promotion(Board &board, Player &player, Square &start, Square &end, char piece);
    /**
     * @brief Makes move with piece
     * @param board Chess board
     * @param player Player
     */
    void make_move(Board &board, Player &player);
    /**
     * @brief Undo move
     * @param board Chess board
     * @param player Player
     */
    void undo_move(Board &board, Player &player);
};