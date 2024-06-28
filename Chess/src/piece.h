#pragma once
#include <iostream>
#include <vector>

#include "position.h"

/**
 * Abstract class for chess pieces
 */
class Piece {
public:
    /**
     * @brief Constructor
     */
    Piece();
    /**
     * @brief Constructor
     * @param name
     * @param color
     * @param pos
     */
    Piece(char name, char color, const Position &pos);
    /**
     * @return Piece's type
     */
    char get_piece() const;
    /**
     * @return Piece's color
     */
    char get_color() const;
    /**
     * @return Piece's position
     */
    Position get_position() const;
    /**
     * @param other Set piece's position
     */
    void set_position(const Position &other);
    /**
     * @brief Validates Position
     * @param other Position
     * @return TRUE -> valid position, FALSE -> no valid position
     */
    bool valid_position(const Position &other) const;
    /**
     * @brief Adds move to possible moves
     * @param dest Position to move
     */
    void add_move(const Position &src);
    /**
     * @brief Clone function
     * @return return cloned bishop
     */
    virtual Piece *clone() const = 0;
    /**
     * @brief Find all possible move based on piece type
     * @return vector of possible moves
     */
    virtual std::vector<Position> *possible_moves() = 0;
    /**
     * @brief Destructor
     */
    virtual ~Piece() = default;
    /**
     * @brief TRUE -> piece moved, FALSE -> piece didn't move
     */
    bool first_move_;
protected:
    /**
     * @brief Piece's type
     */
    char type_;
    /**
     * @brief Piece's color
     */
    char color_;
    /**
     * @brief Piece's position
     */
    Position pos_;
    /**
     * @brief Stores possible moves
     */
    std::vector<Position> moves_;
};
