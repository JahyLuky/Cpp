#pragma once

#include <iostream>

#include "menu.h"
#include "board.h"
#include "piece.h"
#include "player.h"
#include "human_player.h"
#include "ai_player.h"

class Application {
public:
    bool run() const;

    bool tutorial() const;

    bool game(char game_type) const;

private:

};