#pragma once

#include "player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer() = default;

    HumanPlayer(char white_plays);

    bool get_move() override;

    HumanPlayer *clone() const override;

private:
};