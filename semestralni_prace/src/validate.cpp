#include "validate.h"

bool Validate::valid_position(const Position &other) {
    if (other.row_ >= 0 && other.row_ < 8
        && other.col_ >= 0 && other.col_ < 8) {
        return true;
    }
    return false;
}

bool Validate::is_empty(const Square &square) const {
    if (square.piece_ == nullptr) {
        return true;
    }
    return false;
}

bool Validate::is_same_color(const Square &square, const char &color) const {
    if (square.piece_->get_color() == color) {
        return true;
    }
    return false;
}

bool Validate::find_move(const Square &start, const Square &end) {
    std::vector<Position> *possible_moves = start.piece_->possible_moves();
    for (const auto &item: *possible_moves) {
        if (item == end.pos_) {
            return true;
        }
    }
    return false;
}

bool Validate::find_possible_move(const Board &board, Player &player, const Square &start, const Square &end) {
    // Piece cant move to that square
    if (!find_move(start, end)) {
        return false;
    }

    // Pawn moves
    if (tolower(start.piece_->get_piece()) == 'p') {
        int col = start.piece_->get_position().col_;

        // Forward move
        if (end.pos_.col_ == col
            && abs(start.pos_.row_ - end.pos_.row_) == 1
            && is_empty(end)) {
            return true;
        }

        // Double forward move
        if (end.pos_.col_ == col
            && abs(start.pos_.row_ - end.pos_.row_) == 2
            && is_empty(end)) {
            return true;
        }

        if (player.en_passant_) {
            if (player.color_ == 'W') {
                Square pawn = board.squares_[end.pos_.row_ + 1][end.pos_.col_];
                if (!is_empty(pawn)) {
                    return true;
                }
            } else {
                Square pawn = board.squares_[end.pos_.row_ - 1][end.pos_.col_];
                if (!is_empty(pawn)) {
                    return true;
                }
            }
            return false;
        }

        // Left or right take
        if (end.pos_.col_ != col && !is_empty(end)
            && !is_same_color(end, player.color_)) {
            return true;
        }

        // Left or right take (no piece)
        if (end.pos_.col_ != col && is_empty(end)) {
            // En passant
            if (!is_empty(board.end_)
                && tolower(board.end_.piece_->get_piece()) == 'p'
                && abs(board.end_.pos_.row_ - board.start_.pos_.row_) == 2
                && player.start_.col_ != player.end_.col_) {
                if (end.pos_.col_ == board.end_.pos_.col_) {
                    if (player.color_ == 'W') {
                        Square pawn = board.squares_[end.pos_.row_ + 1][end.pos_.col_];
                        if (!is_empty(pawn)) {
                            player.en_passant_ = true;
                            return true;
                        }
                    } else {
                        Square pawn = board.squares_[end.pos_.row_ - 1][end.pos_.col_];
                        if (!is_empty(pawn)) {
                            player.en_passant_ = true;
                            return true;
                        }
                    }
                    return false;
                }
            }
            return false;
        }
        return false;
    }

    // Knight moves
    if ((abs(start.pos_.row_ - end.pos_.row_) == 1
         && abs(start.pos_.col_ - end.pos_.col_) == 2)
        || (abs(start.pos_.row_ - end.pos_.row_) == 2
            && abs(start.pos_.col_ - end.pos_.col_) == 1)) {
        if (!is_empty(end) && is_same_color(end, player.color_)) {
            return false;
        }
        return true;
    }

    int row = 0;
    if (start.pos_.row_ < end.pos_.row_) {
        row = 1;
    } else if (start.pos_.row_ > end.pos_.row_) {
        row = -1;
    }
    int col = 0;
    if (start.pos_.col_ < end.pos_.col_) {
        col = 1;
    } else if (start.pos_.col_ > end.pos_.col_) {
        col = -1;
    }

    Position direction(row, col);

    Position current_pos(start.pos_.row_ + direction.row_, start.pos_.col_ + direction.col_);

    while (current_pos != end.pos_) {
        Square current_square = board.squares_[current_pos.row_][current_pos.col_];
        if (current_square.piece_ != nullptr) {
            // End square = enemy piece
            if (current_pos == end.pos_
                && current_square.piece_->get_color() != player.color_) {
                return true;
            }
            return false;
        }

        current_pos.row_ += direction.row_;
        current_pos.col_ += direction.col_;
    }

    return true;
}

bool Validate::valid_castling(const Board &board, const Square &end) {
    int king_row = end.pos_.row_;
    int king_col = end.pos_.col_;

    // TODO: check in way of castle
    // TODO: kings way must be clean, rooks doesnt

    // Black castling
    if (king_row == 0 && king_col == 2) {
        // Left rook
        const Square &rook = board.squares_[0][0];
        if (rook.piece_ != nullptr && !rook.piece_->first_move_
            && tolower(rook.piece_->get_piece()) == 'r'
            && board.squares_[0][2].piece_ == nullptr
            && board.squares_[0][3].piece_ == nullptr) {
            return true;
        }
    }
    if (king_row == 0 && king_col == 6) {
        // Right rook
        const Square &rook = board.squares_[0][7];
        if (rook.piece_ != nullptr && !rook.piece_->first_move_
            && tolower(rook.piece_->get_piece()) == 'r'
            && board.squares_[0][5].piece_ == nullptr) {
            return true;
        }
    }

    // White castling
    if (king_row == 7 && king_col == 2) {
        // Left rook
        const Square &rook = board.squares_[7][0];
        if (rook.piece_ != nullptr && !rook.piece_->first_move_
            && tolower(rook.piece_->get_piece()) == 'r'
            && board.squares_[7][2].piece_ == nullptr
            && board.squares_[7][3].piece_ == nullptr) {
            return true;
        }
    }
    if (king_row == 7 && king_col == 6) {
        // Right rook
        const Square &rook = board.squares_[7][7];
        if (rook.piece_ != nullptr && !rook.piece_->first_move_
            && tolower(rook.piece_->get_piece()) == 'r'
            && board.squares_[7][5].piece_ == nullptr) {
            return true;
        }
    }
    return false;
}

bool Validate::validate_move(const Board &board, Player &player, bool output) {
    // Starting position of piece
    const Square &start = board.squares_[player.start_.row_][player.start_.col_];

    // Final position of piece
    const Square &end = board.squares_[player.end_.row_][player.end_.col_];

    // Cant move with empty square
    if (start.piece_ == nullptr) {
        if (output) {
            std::cout << "Choose valid piece!" << std::endl;
        }
        return false;
    }

    // Moving piece on square where is already our piece
    if (end.piece_ != nullptr
        && end.piece_->get_color() == player.color_) {
        if (output) {
            std::cout << "Can't take our piece!" << std::endl;
        }
        return false;
    }

    // Moving piece of opposite color
    if (start.piece_->get_color() != player.color_) {
        if (output) {
            std::cout << "Choose piece with your color!" << std::endl;
        }
        return false;
    }

    // Moving king who hasn't moved yet
    if (tolower(start.piece_->get_piece()) == 'k' && !start.piece_->first_move_) {
        if (abs(player.start_.col_ - player.end_.col_) == 2) {
            player.castling_ = valid_castling(board, end);
        } else {
            start.piece_->first_move_ = true;
        }
    }

    bool found_move = find_possible_move(board, player, start, end);

    // No valid move found
    if (!found_move) {
        if (output) {
            std::cout << "Cant move piece to that square!" << std::endl;
        }
        return false;
    }

    return true;
}