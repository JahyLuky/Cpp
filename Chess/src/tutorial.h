#pragma once
#include <iostream>

#include "file_manager.h"
#include "human_player.h"
#include "application.h"

/**
 * Class that takes care of tutorial for chess
 */
class Tutorial {
public:
    /**
     * @brief Default constructor
     */
    Tutorial() = default;
    /**
     * @brief Prints menu
     */
    void print_menu() const;
    /**
     * @brief Start tutorial
     * @param board Chess board
     * @param file_path Path to file
     * @return TRUE -> tutorial started, FALSE -> otherwise
     */
    bool start_tutorial(Board &board, const std::string &file_path);
    /**
     * @brief Runs tutorial
     */
    void run_tutorial();
};