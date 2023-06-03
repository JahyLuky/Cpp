#pragma once

#include <iostream>
#include "file_manager.h"
#include "human_player.h"

class Tutorial {
public:
    Tutorial() = default;

    void place_piece_on_board(std::vector<Square> &pieces, char piece, const Position &pos);

    void fill_pieces(Board &board, std::vector<Square> &pieces, char FEN_piece, int FEN_cnt, Position &pos);

    bool read_file(std::ifstream &ifs);

    void run();

private:
};