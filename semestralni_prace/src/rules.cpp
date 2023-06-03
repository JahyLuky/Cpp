#include "rules.h"

bool Rules::valid_position(const Position &other) {
    if (other.row_ >= 0 && other.row_ < BOARD_SIZE
        && other.col_ >= 0 && other.col_ < BOARD_SIZE) {
        return true;
    }
    return false;
}

bool Rules::is_empty(const Square &square) const {
    if (square.piece_ == nullptr) {
        return true;
    }
    return false;
}

bool Rules::is_same_color(const Square &square, const char &color) const {
    if (square.piece_->get_color() == color) {
        return true;
    }
    return false;
}

bool find_move(const Square &start, const Square &end) {
    std::vector<Position> *possible_moves = start.piece_->possible_moves();
    for (const auto &item: *possible_moves) {
        if (item == end.pos_) {
            return true;
        }
    }
    return false;
}

bool Rules::find_possible_move(const Board &board, Player &player, const Square &start, const Square &end) const {
    // Piece cant move to that square
    if (!find_move(start, end)) {
        return false;
    }

    // Pawn moves
    if (tolower(start.piece_->get_piece()) == 'p') {
        int col = start.piece_->get_position().col_;

        // Forward move
        if (end.pos_.col_ == col
            && is_empty(end)) {
            return true;
        }

        // Left or right take
        if (end.pos_.col_ != col && !is_empty(end)
            && !is_same_color(end, player.color_)) {
            return true;
        }

        // En passant
        if (tolower(board.end_.piece_->get_piece()) == 'p'
            && abs(board.end_.pos_.row_ - board.start_.pos_.row_) == 2) {
            if (end.pos_.col_ == board.end_.pos_.col_) {
                player.en_passant_ = true;
                return true;
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

bool valid_castling(const Board &board, const Square &end) {
    int king_row = end.pos_.row_;
    int king_col = end.pos_.col_;

    // TODO: check in way of castle
    // TODO: kings way must be clean, rooks doesnt

    // Black castling
    if (king_row == 0 && king_col == 2) {
        // Left rook
        const Square &rook = board.squares_[0][0];
        if (rook.piece_ != nullptr && !rook.piece_->first_move_
            && tolower(rook.piece_->get_piece()) == 'r') {
            return true;
        }
    }
    if (king_row == 0 && king_col == 6) {
        // Right rook
        const Square &rook = board.squares_[0][7];
        if (rook.piece_ != nullptr && !rook.piece_->first_move_
            && tolower(rook.piece_->get_piece()) == 'r') {
            return true;
        }
    }

    // White castling
    if (king_row == 7 && king_col == 2) {
        // Left rook
        const Square &rook = board.squares_[7][0];
        if (rook.piece_ != nullptr && !rook.piece_->first_move_
            && tolower(rook.piece_->get_piece()) == 'r') {
            return true;
        }
    }
    if (king_row == 7 && king_col == 6) {
        // Right rook
        const Square &rook = board.squares_[7][7];
        if (rook.piece_ != nullptr && !rook.piece_->first_move_
            && tolower(rook.piece_->get_piece()) == 'r') {
            return true;
        }
    }
    return false;
}

bool Rules::validate_move(const Board &board, Player &player) {
    // Starting position of piece
    const Square &start = board.squares_[player.start_.row_][player.start_.col_];

    // Final position of piece
    const Square &end = board.squares_[player.end_.row_][player.end_.col_];

    // TODO: change IF for better reading (is_empty, ...)

    // Cant move with empty square
    if (start.piece_ == nullptr) {
        std::cout << "Choose valid piece!" << std::endl;
        return false;
    }

    // Moving piece on square where is already our piece
    if (end.piece_ != nullptr
        && end.piece_->get_color() == player.color_) {
        std::cout << "Can't take our piece!" << std::endl;
        return false;
    }

    // Moving piece of opposite color
    if (start.piece_->get_color() != player.color_) {
        std::cout << "Choose piece with your color!" << std::endl;
        return false;
    }

    // Moving king who hasn't moved yet
    if (tolower(start.piece_->get_piece()) == 'k' && !start.piece_->first_move_) {
        player.castling_ = valid_castling(board, end);
    }

    bool found_move = find_possible_move(board, player, start, end);

    // No valid move found
    if (!found_move) {
        std::cout << "Cant move piece to that square!" << std::endl;
        return false;
    }

    return true;
}

const std::vector<Position> move_directions = {
        Position(1, 0),
        Position(0, 1),
        Position(-1, 0),
        Position(0, -1),
        Position(1, 1),
        Position(-1, -1),
        Position(-1, 1),
        Position(1, -1)
};

const std::vector<Position> knight_directions = {
        Position(-2, -1),
        Position(-2, 1),
        Position(-1, -2),
        Position(-1, 2),
        Position(1, -2),
        Position(1, 2),
        Position(2, -1),
        Position(2, 1)
};

bool Rules::checked(const Board &board, const Position &pos, char color) {
    int row = pos.row_;
    int col = pos.col_;
    int row_cnt = 0, col_cnt = 0;

    bool checked = false;
    // Looks out for diagonal, row and column checks
    for (const auto &direction: move_directions) {
        row_cnt = row + direction.row_;
        col_cnt = col + direction.col_;

        Position position(row_cnt, col_cnt);

        while (valid_position(position)) {
            Square square = board.squares_[position.row_][position.col_];
            if (square.piece_ != nullptr) {
                if (square.piece_->get_color() != color) {
                    // Check if opponent's piece can attack your king
                    std::vector<Position> *possible_moves = square.piece_->possible_moves();

                    for (const auto &item: *possible_moves) {
                        if (item.row_ == pos.row_
                            && item.col_ == pos.col_) {
                            if (tolower(square.piece_->get_piece()) == 'p'
                                && square.piece_->get_position().col_ == pos.col_) {
                                checked = false;
                            } else {
                                checked = true;
                            }
                        }
                    }
                    break;
                }
            }
            // Square was empty, look at next
            position.row_ += direction.row_;
            position.col_ += direction.col_;
        }
    }

    // Looks out for knight checks
    row_cnt = 0, col_cnt = 0;
    for (const auto &knightDirection: knight_directions) {
        row_cnt = row + knightDirection.row_;
        col_cnt = col + knightDirection.col_;

        Position position(row_cnt, col_cnt);

        if (valid_position(position)) {
            Square square = board.squares_[position.row_][position.col_];
            if (!is_empty(square)
                && tolower(square.piece_->get_piece()) == 'n'
                && !is_same_color(square, color)) {
                checked = true;
                break;
            }
        }
    }

    return checked;
}