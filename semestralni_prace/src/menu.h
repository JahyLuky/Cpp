#pragma once
#include <iostream>
#include "application.h"
#include "tutorial.h"

/**
 * @brief Represents MENU
 */
class Menu {
public:
    /**
     * @brief Prints menu
     */
    void print_menu() const;
    /**
     * @brief Starts game with 2 human players
     */
    void start_game();
    /**
     * @brief Starts game with AI
     * @return TRUE -> game finished, FALSE -> game didn't finish
     */
    bool start_AI_game();
    /**
     * @brief Load game
     * @return TRUE -> loading was correct, FALSE -> otherwise
     */
    bool load_game();
    /**
     * @brief Runs menu
     */
    void run();
};