#pragma once

#include "player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer() = default;

    bool make_move(Board &src, const Position &old_pos, const Position &new_pos) override;

    HumanPlayer *clone() const override;

private:
};