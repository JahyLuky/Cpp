#pragma once
#include <iostream>
#include <vector>
#include "board.h"

enum draw_status {
    resign = -1,        // Indicates a resignation
    no_draw_asked = 0,  // No draw has been requested
    draw_asked = 1,     // A draw has been requested
    draw = 2,           // The game ends in a draw
    no_draw = 3,        // The draw request has been declined
    save = 4            // Special value indicating saving the file
};

/**
 * @brief Abstract class for player
 */
class Player {
public:
    /**
     * @brief Default Constructor
     */
    Player() = default;
    /**
     * @brief Constructor
     * @param color Player's color
     */
    Player(char color);
    /**
     * @brief Fill pieces on chess board
     * @param board Chess board
     */
    void fill_pieces(Board &board);
    /**
     * @brief Update piece's position on chess board
     * @param start Start position of player's move
     * @param end End position of player's move
     */
    void update_pieces(const Position &start, const Position &end);
    /**
     * @brief Delete piece from pieces_pos_
     * @param pos Position of piece to be deleted
     */
    void delete_piece(const Position &pos);
    /**
     * @brief Checks player's pawn promotion
     * @param piece Chess piece
     * @return TRUE -> correct promotion, FALSE -> incorrect promotion
     */
    virtual bool get_promotion(char &piece) = 0;
    /**
     * @brief Gets player's move
     * @param board chess board
     * @return TRUE -> valid move, FALSE -> invalid move
     */
    virtual bool get_move(Board &board) = 0;
    /**
     * @brief PLayer's color
     */
    char color_;
    /**
     * @brief Stores start position of player's move
     */
    Position start_;
    /**
     * @brief Stores end position of player's move
     */
    Position end_;
    /**
     * @brief Stores kings positions
     */
    Position king_;
    /**
     * @brief Checks if castling is possible
     */
    bool castling_;
    /**
     * @brief Checks if en passant is possible
     */
    bool en_passant_;
    /**
     * @brief Checks if player asked for draw
     */
    draw_status ask_draw_;
    /**
     * @brief Stores position of all player's pieces
     */
    std::vector<Position> pieces_pos_;
};