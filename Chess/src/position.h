#pragma once
#include <iostream>

/**
 * Represents position on chess board
 */
struct Position {
    /**
     * @brief Row
     */
    int row_;
    /**
     * @brief Col
     */
    int col_;
    /**
     * @brief Default constructor
     */
    Position() = default;
    /**
     * @brief Constructor
     * @param a Row
     * @param b Col
     */
    Position(int a, int b);
    /**
     * @brief Copy other
     * @param other
     */
    Position(const Position &other);
    /**
     * @brief Operator =
     * @param other
     * @return this
     */
    Position &operator=(const Position &other);
    /**
     * @brief Operator ==
     * @param other
     * @return TRUE -> positions are equal, FALSE -> otherwise
     */
    bool operator==(const Position &other) const;
    /**
     * @brief Operator -=
     * @param other Position to be compared
     * @return TRUE -> positions are not equal, FALSE -> otherwise
     */
    bool operator!=(const Position &other) const;
    /**
     * @brief Destructor
     */
    ~Position() = default;
};
