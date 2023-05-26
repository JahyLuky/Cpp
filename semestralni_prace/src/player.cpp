#include "player.h"

Player::Player(char color)
        : color_(color), castling_(false) {
    if (color == 'W') {
        this->king_.row_ = 7;
        this->king_.col_ = 4;
    } else {
        this->king_.row_ = 0;
        this->king_.col_ = 4;
    }
}

int Player::coordinates_to_int(char c) {
    switch (tolower(c)) {
        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
        default:
            return -1;
    }
}

int Player::check_col(char c) {
    int num = c - '0';
    if (num < 1 || num > 8) {
        return -1;
    }
    return num;
}

bool Player::extract_input(const std::string &start, const std::string &end, Position &old_pos, Position &new_pos) {
    int col = coordinates_to_int(start[0]);
    int row = check_col(start[1]);

    int col2 = coordinates_to_int(end[0]);
    int row2 = check_col(end[1]);

    if (row == -1 || col == -1 || row2 == -1 || col2 == -1
        || start.length() > 2 || end.length() > 2) {
        std::cout << "Coordinates out of range!" << std::endl;
        return false;
    }

    old_pos.row_ = 8 - row;
    old_pos.col_ = col;

    new_pos.row_ = 8 - row2;
    new_pos.col_ = col2;

    return true;
}

void Player::print_captures() const {
    std::cout << this->color_ << ": ";
    for (const auto &item: this->captures_) {
        std::cout << item << ", ";
    }
}
