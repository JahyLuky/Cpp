#include "human_player.h"

HumanPlayer::HumanPlayer(char color)
        : Player(color) {}

bool HumanPlayer::get_move() {
    std::string start, end;

    // Get positions
    std::cin >> start;
    std::cin >> end;

    // Check "ctrl+d"
    if (std::cin.eof()) {
        std::cout << "Input failed!" << std::endl;
        return false;
    }

    if (!extract_input(start, end, this->start_, this->end_)) {
        return false;
    }

    return true;
}

HumanPlayer *HumanPlayer::clone() const {
    return new HumanPlayer(*this);
}
