#pragma once

#include <iostream>
#include <fstream>

#include "board.h"

class Tutorial {
public:
    Tutorial() = default;

    void place_piece_on_board(std::vector<Square> &pieces, char piece, const Position &pos);

    bool read_file(std::ifstream &ifs);

    void run();

private:
};