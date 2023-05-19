#pragma once

#include "player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer() = default;

    HumanPlayer(char white_plays);

    bool make_move(Board &src, const Position &old_pos, const Position &new_pos) override;

    HumanPlayer *clone() const override;

private:
    char white_plays_;
};