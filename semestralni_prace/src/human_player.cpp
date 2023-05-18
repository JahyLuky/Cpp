#include "human_player.h"

bool HumanPlayer::make_move(Board &src, const Position &old_pos, const Position &new_pos) {
    // Starting position of piece
    int old_place = (old_pos.row_ * 8) + old_pos.col_;

    // empty square
    if (src.squares_[old_place].piece_ == nullptr) {
        std::cout << "Choose valid piece!" << std::endl;
        return false;
    }

    // Final position of piece
    int new_place = (new_pos.row_ * 8) + new_pos.col_;


    int move = 0;
    bool found_move = false;
    std::vector<Position> *tmp = src.squares_[old_place].piece_->possible_moves();
    // TODO: validate 'possible_moves' in its piece method
    // check if new_pos is a possible move
    for (const auto &i: *tmp) {
        move = (i.row_ * 8) + i.col_;
        if (move == new_place) {
            found_move = true;
            break;
        }
    }

    // no valid move found
    if (!found_move) {
        std::cout << "Cant move piece to that square!" << std::endl;
        return false;
    }


    // Copy piece "name"
    char new_name = src.squares_[old_place].piece_->get_piece();
    // Copy piece color
    char new_color = src.squares_[old_place].piece_->get_color();
    // Create new square with moved piece
    src.squares_[new_place].pos_ = new_pos;
    // TODO: change 'pawn' to correct piece
    src.squares_[new_place].piece_ = std::make_unique<Pawn>
            (new_name, new_color, Position(new_pos.row_, new_pos.col_));

    // Empty starting square
    src.squares_[old_place].piece_ = nullptr;
    return true;
}

HumanPlayer *HumanPlayer::clone() const {
    return new HumanPlayer(*this);
}
