#include "player.h"

Player::Player(char color)
        : color_(color), castling_(false), en_passant_(false) {
    if (color == 'W') {
        king_.row_ = 7;
        king_.col_ = 4;
    } else {
        king_.row_ = 0;
        king_.col_ = 4;
    }
    castling_ = false;
    en_passant_ = false;
    ask_draw_ = no_draw_asked;
    start_.row_ = 0;
    start_.col_ = 0;
    end_.row_ = 0;
    end_.col_ = 0;
}

void Player::fill_pieces(Board &board) {
    for (const auto &row: board.squares_) {
        for (const auto &col: row) {
            Square &square = board.squares_[col.pos_.row_][col.pos_.col_];

            if (square.piece_ != nullptr && square.piece_->get_color() == color_) {
                pieces_pos_.emplace_back(col.pos_.row_, col.pos_.col_);

                if (color_ == 'W') {
                    if (square.piece_->get_piece() == 'K') {
                        king_ = col.pos_;
                        continue;
                    }
                    if (square.piece_->get_piece() == 'P' && col.pos_.row_ != 6) {
                        square.piece_->first_move_ = true;
                    } else {
                        square.piece_->first_move_ = false;
                    }
                } else {
                    if (square.piece_->get_piece() == 'k') {
                        king_ = col.pos_;
                        continue;
                    }
                    if (square.piece_->get_piece() == 'p' && col.pos_.row_ != 1) {
                        square.piece_->first_move_ = true;
                    } else {
                        square.piece_->first_move_ = false;
                    }
                }
            }
        }
    }
}

void Player::update_pieces(const Position &start, const Position &end) {
    for (auto &pieces_po: pieces_pos_) {
        if (pieces_po == start) {
            pieces_po = end;
            break;
        }
    }
}

void Player::delete_piece(const Position &start) {
    for (auto it = pieces_pos_.begin(); it != pieces_pos_.end(); ++it) {
        if (*it == start) {
            pieces_pos_.erase(it);
            break;
        }
    }
}
