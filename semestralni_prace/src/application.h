#pragma once

#include <iostream>

#include "rules.h"
#include "human_player.h"
#include "ai_player.h"

class Application {
public:
    void make_move(Board &board, Player &player) const;

    bool play_move(Board &board, Player &player) const;

    bool game() const;

private:
};
