#include "human_player.h"

HumanPlayer::HumanPlayer(char white_plays)
: white_plays_(white_plays) {}

bool HumanPlayer::make_move(Board &src, const Position &old_pos, const Position &new_pos) {
    // Starting position of piece
    int old_place = (old_pos.row_ * 8) + old_pos.col_;

    // empty square
    if (src.squares_[old_place].piece_ == nullptr) {
        std::cout << "Choose valid piece!" << std::endl;
        return false;
    }

    // moving with piece of opposite color
    if (src.squares_[old_place].piece_->get_color() != this->white_plays_) {
        std::cout << "Choose piece with your color!" << std::endl;
        return false;
    }

    // Final position of piece
    int new_place = (new_pos.row_ * 8) + new_pos.col_;

    int move = 0;
    bool found_move = false;
    std::vector<Position> *tmp = src.squares_[old_place].piece_->possible_moves();

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

    // TODO: change 'pawn' to correct piece

    // TODO: how to access child from abstract class
    if (tolower(src.squares_[old_place].piece_->get_piece()) == 'p'){
        src.squares_[old_place].piece_->first_move_ = false;
    }

    // TODO: if piece was taken, add it to Players vector of taken pieces

    // move piece to new position
    src.squares_[new_place].piece_ = std::unique_ptr<Piece>(src.squares_[old_place].piece_->clone());

    // change coor to new position
    src.squares_[new_place].piece_->set_position(Position(new_pos.row_, new_pos.col_));

    // Empty starting square
    src.squares_[old_place].piece_ = nullptr;
    return true;
}

HumanPlayer *HumanPlayer::clone() const {
    return new HumanPlayer(*this);
}
