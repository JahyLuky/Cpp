#pragma once

#include <iostream>

#include "rules.h"
#include "human_player.h"
#include "ai_player.h"

class Application {
public:
    void print_color(bool white_plays);

    void move_piece(Square &start, Square &end);

    void handle_castling(Board &board, Player &player, Square &start, Square &end);

    void make_move(Board &board, Player &player);

    void undo_move(Board &board, Player &player);

    bool play_move(Board &board, Player &player);

    bool game();

private:
};
