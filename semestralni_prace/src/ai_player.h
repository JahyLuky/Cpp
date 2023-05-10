#pragma once

#include "player.h"

class AIPlayer : public Player {
public:
    AIPlayer() = default;

    bool make_move(Board &src, const Position &old_pos, const Position &new_pos) override;

    AIPlayer *clone() const override;

private:
};