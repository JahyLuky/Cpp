#pragma once
#include <iostream>
#include <vector>
#include "square.h"
#include "position.h"
#include "pawn.h"
#include "queen.h"
#include "king.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"

enum game_result {
    game_not_finished = 0,  // The game is still in progress
    white_wins = 1,         // White player wins
    black_wins = 2,         // Black player wins
    tie = 3,                // The game ends in a tie
    pat = 4,                // The game ends in a pat
    tutorial = 5,           // Special value indicating tutorial
    save_game = 6           // Special value indicating saving the game
};

/**
 * @brief Represents classical chess board
 */
class Board {
public:
    Board();
    /**
     * Initialize classical chess board
     * @return Board with pieces
     */
    void init_board();
    /**
     * @brief Prints chess board
     */
    void print_board() const;
    /**
     * @brief Resets the board
     */
    void reset_board();
    /**
     * @brief TRUE -> white plays, FALSE -> black plays
     */
    bool white_playes;
    /**
     * @brief Represents game status
     */
    game_result game_over;
    /**
     * @brief Stores each square of chess board
     */
    std::vector<std::vector<Square>> squares_;
    /**
     * @brief Stores taken piece
     */
    Square taken_;
    /**
     * @brief Stores start square from move
     */
    Square start_;
    /**
     * @brief Stores end square from move
     */
    Square end_;
    /**
     * @brief Half move counter for 50-move rule
     */
    int halfmoves_;
    /**
     * @brief Move counter
     */
    int fullmoves_;
};
