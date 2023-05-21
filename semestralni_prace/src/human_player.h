#pragma once

#include "player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer() = default;

    HumanPlayer(char white_plays);

    bool validate_move(Board &board) const;

    bool get_move(Board &board) override;

    HumanPlayer *clone() const override;

private:
};