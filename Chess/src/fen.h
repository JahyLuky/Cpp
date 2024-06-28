#pragma once
#include <iostream>
#include "board.h"
#include "player.h"

/**
 * @brief Represents FEN notation
 */
class FEN {
public:
    /**
     * @brief Default Constructor
     */
    FEN() = default;
    /**
     * @brief Gets types of piece
     * @param piece Chess piece
     * @return Chess piece
     */
    std::string get_piece_notation(const Piece *piece);
    /**
     * @brief Save piece
     * @param board Chess board
     * @return return chess pieces in FEN notation
     */
    std::string save_pieces(const Board &board);
    /**
     * @brief Gets FEN notation
     * @param board Chess board
     * @param white White player
     * @param black Black player
     * @return FEN notation
     */
    std::string get_FEN(Board &board, const Player &white, const Player &black);
    /**
     * @brief Corrects FEN pieces
     * @param str String to correct
     * @return TRUE -> correct, FALSE -> incorrect
     */
    bool correct_FEN_pieces(const std::string &str);
    /**
     * @brief Place pieces on chess board
     * @param pieces Vector of chess pieces
     * @param piece Chess piece
     * @param pos
     * @param white
     * @param black
     */
    void place_piece_on_board(std::vector<Square> &pieces, char piece, const Position &pos, Player &white, Player &black);
    /**
     * @brief Fills chess pieces into board
     * @param board Chess board
     * @param fen FEN notation
     * @param white White player
     * @param black Black player
     */
    void fill_pieces(Board &board, std::string &fen, Player &white, Player &black);
    /**
     * @brief Reads FEN notation
     * @param fen FEN notation to read
     * @param board Chess board
     * @param white White player
     * @param black Black player
     */
    void read_FEN(const std::string &fen, Board &board, Player &white, Player &black);
};