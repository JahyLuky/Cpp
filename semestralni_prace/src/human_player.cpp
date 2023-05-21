#include "human_player.h"

HumanPlayer::HumanPlayer(char color)
        : Player(color) {}

bool HumanPlayer::validate_move(Board &board) const {
    // Starting position of piece
    Square &start = board.squares_[this->start_.row_][this->start_.col_];

    // Final position of piece
    Square &end = board.squares_[this->end_.row_][this->end_.col_];

    // cant move with empty square
    if (start.piece_ == nullptr) {
        std::cout << "Choose valid piece!" << std::endl;
        return false;
    }

    // moving piece on square where is already our piece
    if (end.piece_ != nullptr
        && end.piece_->get_color() == this->color_) {
        std::cout << "Can't take our piece!" << std::endl;
        return false;
    }

    // moving piece of opposite color
    if (start.piece_->get_color() != this->color_) {
        std::cout << "Choose piece with your color!" << std::endl;
        return false;
    }

    bool found_move = false;
    std::vector<Position> *possible_moves = start.piece_->possible_moves(board);
    for (const auto &item: *possible_moves) {
        if (item.row_ == end.pos_.row_
            && item.col_ == end.pos_.col_) {
            found_move = true;
            break;
        }
    }

    // No valid move found
    if (!found_move) {
        std::cout << "Cant move piece to that square!" << std::endl;
        return false;
    }

    return true;
}

bool HumanPlayer::get_move(Board &board) {
    std::string start, end;

    // load positions
    std::cin >> start;
    std::cin >> end;

    if (!extract_input(start, end, this->start_, this->end_)) {
        return false;
    }

    if (!validate_move(board)) {
        return false;
    }

    return true;
}

HumanPlayer *HumanPlayer::clone() const {
    return new HumanPlayer(*this);
}
