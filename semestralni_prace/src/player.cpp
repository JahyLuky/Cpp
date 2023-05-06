#include "player.h"


void Player::make_move(Board &src, const Position &old_pos, const Position &new_pos) {
    // Starting position of piece
    int old_place = (old_pos.row_ * 8) + old_pos.col_;
    // Final position of piece
    int new_place = (new_pos.row_ * 8) + new_pos.col_;

    if (src.squares_[old_place].piece_ != nullptr) {
        // Copy piece "name"
        char new_name = src.squares_[old_place].piece_->get_piece();
        // Copy piece color
        char new_color = src.squares_[old_place].piece_->get_color();
        // Create new square with moved piece
        src.squares_[new_place].pos_ = new_pos;
        src.squares_[new_place].piece_ = std::make_unique<Pawn>//(std::move(p));
                (new_name, new_color, Position(new_pos.row_, new_pos.col_));


    }
    // Empty starting square
    src.squares_[old_place].piece_ = nullptr;
}
