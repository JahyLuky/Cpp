#pragma once

#include "player.h"

class AIPlayer : public Player {
public:
    AIPlayer() = default;

    bool get_move(Board &src) override;

    AIPlayer *clone() const override;

private:
};