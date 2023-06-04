#pragma once

#include <iostream>
#include <fstream>

#include "board.h"
#include "player.h"

class File_manager {
public:

    /**
     *
     * @param pieces
     * @param piece
     * @param pos
     */
    void place_piece_on_board(std::vector<Square> &pieces, char piece, const Position &pos);

    /**
     *
     * @param board
     * @param pieces
     * @param FEN_piece
     * @param FEN_cnt
     * @param pos
     */
    void fill_pieces(Board &board, std::string &fen);

    void print_FEN(std::vector<std::string> &fields);

    void readFEN(const std::string &fen, Board &board, Player &white, Player &black);

    /**
     *
     * @param file_path
     * @param board
     * @param white
     * @param black
     * @return
     */
    bool open_file(const std::string &file_path, Board &board, Player &white, Player &black);

private:
};