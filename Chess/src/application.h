#pragma once
#include <iostream>
#include "rules.h"
#include "move.h"
#include "game_rules.h"

/**
 * @brief Represents App that handles chess game
 */
class Application {
public:
    /**
     * @brief Prints who plays next
     * @param white_plays TRUE -> white plays, FALSE -> black plays
     */
    void print_color(bool white_plays) const;
    /**
     * @brief
     * @param board Chess board
     * @param player
     * @return TRUE -> move was correct, FALSE -> wrong move
     */
    bool play_move(Board &board, Player &player);
    /**
     * @brief Prints result of game (checkmate, tie, pat, game not finished)
     * @param board Chess board
     */
    void print_results(const Board &board) const;
    /**
     * @brief Changes turn of game
     * @param board Chess board
     * @param white White player
     * @param black Black player
     * @param wrong_move_cnt Counter for wrong moves
     * @param is_tutorial TRUE -> game ends after tutorial, FALSE -> normal game
     * @return TRUE -> turn was changed, FALSE -> turn wasn't changed
     */
    bool change_turns(Board &board, Player &white, Player &black, int &wrong_move_cnt, bool is_tutorial);
    /**
     * @brief Starts and handles game
     * @param board Chess board
     * @param white White player
     * @param black Black player
     * @param is_tutorial TRUE -> game ends after tutorial, FALSE -> normal game
     */
    void game(Board &board, Player &white, Player &black, bool is_tutorial);
};
