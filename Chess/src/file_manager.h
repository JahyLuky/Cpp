#pragma once
#include <iostream>
#include <fstream>
#include <set>
#include "board.h"
#include "player.h"

/**
 * @brief Represents file manager
 */
class File_manager {
public:
    /**
     * @brief Default Constructor
     */
    File_manager() = default;
    /**
     * @brief Checks if file name ends with .txt
     * @param str File name
     * @return True -> file ends with .txt, FALSE -> file doesn't
     */
    bool ends_with_txt(const std::string &str);
    /**
     * @brief List all files in directory
     * @param dir_file_names Vector of file names
     * @param dir_path Directory path
     */
    void list_directory(std::set<std::string> &dir_file_names, const std::string &dir_path);
    /**
     * @brief Saves file
     * @param board Chess board
     * @param white White player
     * @param black Black player
     */
    void save_file(Board &board, Player &white, Player &black);
    /**
     * @brief Opens file
     * @param file_path File path
     * @param board Chess board
     * @param white White player
     * @param black Black player
     */
    void open_file(const std::string &file_path, Board &board, Player &white, Player &black);
};