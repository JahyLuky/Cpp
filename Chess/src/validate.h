#pragma once
#include <iostream>
#include "board.h"
#include "player.h"

/**
 * Class that validates chess moves
 */
class Validate {
public:
    /**
     * @brief Validates position
     * @param other
     * @return TRUE -> valid position, FALSE -> invalid position
     */
    bool valid_position(const Position &other);
    /**
     * @brief Checks if square is empty
     * @param square
     * @return TRUE -> empty, FALSE -> not empty
     */
    bool is_empty(const Square &square) const;
    /**
     * @brief Checks if piece is same color as player
     * @param square
     * @param color
     * @return TRUE -> same color, FALSE -> not same color
     */
    bool is_same_color(const Square &square, const char &color) const;
    /**
     * @brief Finds if move is legit
     * @param start
     * @param end
     * @return TRUE -> legit move, FALSE -> no legit move
     */
    bool find_move(const Square &start, const Square &end);
    /**
     * @brief Finds all possible moves for piece
     * @param board Chess board
     * @param player Player
     * @param start Start square
     * @param end End square
     * @return TRUE -> move found, FALSE -> no move found
     */
    bool find_possible_move(const Board &board, Player &player, const Square &start, const Square &end);
    /**
     * @brief Checks if castling is valid
     * @param board Chess board
     * @param end End square
     * @return TRUE -> valid castling, FALSE -> invalid castling
     */
    bool valid_castling(const Board &board, const Square &end);
    /**
     * @brief Validates move
     * @param board Chess board
     * @param player Player
     * @param output TRUE -> output on, FALSE -> no output
     * @return TRUE -> valid move, FALSE -> invalid move
     */
    bool validate_move(const Board &board, Player &player, bool output);
};