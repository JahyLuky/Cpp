#pragma once

#include <iostream>

#include "position.h"

class Input_handler {
public:
    /**
     * @brief Default Constructor
     */
    Input_handler() = default;
    /**
     * @brief Converts char to int
     * @param c
     * @return Number value of char
     */
    int coordinates_to_int(char c);
    /**
     * @brief Converts int to char
     * @param c
     * @return Char value of int
     */
    char int_to_coordinates(int c);
    /**
     * @brief Checks if char is <0, 8>
     * @param c
     * @return Number value of char
     */
    int check_col(char c);
    /**
     * @brief Extract input
     * @param start First input
     * @param end Second input
     * @param old_pos -> convert and saves start position
     * @param new_pos -> convert and saves end position
     * @return true -> saves correct position
     * @return false -> wrong position
     */
    bool extract_input(const std::string &start, Position &old_pos);
};