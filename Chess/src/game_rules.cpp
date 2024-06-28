#include "game_rules.h"
#include "rules.h"

void GameRules::update_threefold(Board &board, Player &white, Player &black, std::vector<Position> &white_last_moves,
                                 std::vector<Position> &black_last_moves) {
    if (board.taken_.piece_ != nullptr) {
        // Piece was taken, reset Threefold repetition
        white_last_moves.clear();
        black_last_moves.clear();
    } else {
        // No piece taken, add moves for Threefold repetition
        if (!board.white_playes) {
            white_last_moves.emplace_back(white.start_);
            white_last_moves.emplace_back(white.end_);
        } else {
            black_last_moves.emplace_back(black.start_);
            black_last_moves.emplace_back(black.end_);
        }
    }
}

bool GameRules::threefold(const std::vector<Position> &white_last_moves,
                          const std::vector<Position> &black_last_moves) const {
    int white = 0, black = 0;
    Position last(-1, -1);
    for (const auto &item: white_last_moves) {
        if (last.row_ == item.row_ && last.col_ == item.col_) {
            white++;
        }
        last = item;
    }
    last.row_ = -1;
    last.col_ = -1;
    for (const auto &item: black_last_moves) {
        if (last.row_ == item.row_ && last.col_ == item.col_) {
            black++;
        }
        last = item;
    }
    if (black == white && white >= 3 && black >= 3) {
        return true;
    }
    return false;
}

bool GameRules::dead_position(Board &board, const Player &white, const Player &black) {
    if (white.pieces_pos_.size() < 3
        && black.pieces_pos_.size() < 3) {
        int white_no_pieces = 0;
        int black_no_pieces = 0;
        for (const auto &item: white.pieces_pos_) {
            Square &tmp = board.squares_[item.row_][item.col_];
            if (tmp.piece_->get_piece() == 'Q'
                || tmp.piece_->get_piece() == 'P'
                || tmp.piece_->get_piece() == 'R'
                || tmp.piece_->get_piece() == 'B'
                || tmp.piece_->get_piece() == 'N') {
                return false;
            }
            if (tmp.piece_->get_piece() == 'K') {
                white_no_pieces += 1;
            }
            if (tmp.piece_->get_piece() == 'B') {
                white_no_pieces += 2;
            }
            if (tmp.piece_->get_piece() == 'N') {
                white_no_pieces += 3;
            }
        }
        for (const auto &item: black.pieces_pos_) {
            Square &tmp = board.squares_[item.row_][item.col_];
            if (tmp.piece_->get_piece() == 'q'
                || tmp.piece_->get_piece() == 'p'
                || tmp.piece_->get_piece() == 'r'
                || tmp.piece_->get_piece() == 'b'
                || tmp.piece_->get_piece() == 'n') {
                return false;
            }
            if (tmp.piece_->get_piece() == 'k') {
                black_no_pieces += 1;
            }
            if (tmp.piece_->get_piece() == 'b') {
                black_no_pieces += 2;
            }
            if (tmp.piece_->get_piece() == 'n') {
                black_no_pieces += 3;
            }
        }
        if ((white_no_pieces == 1 && black_no_pieces == 1)
            || (white_no_pieces == 1 && black_no_pieces == 3)
            || (white_no_pieces == 1 && black_no_pieces == 4)
            || (white_no_pieces == 3 && black_no_pieces == 3)
            || (white_no_pieces == 3 && black_no_pieces == 1)
            || (white_no_pieces == 4 && black_no_pieces == 1)
                ) {
            return true;
        }
    }
    return false;
}

void GameRules::half_moves(Board &board, Player &white, Player &black) {
    if (board.taken_.piece_ != nullptr) {
        board.halfmoves_ = 0;
    } else {
        board.halfmoves_++;
    }
    int row = white.end_.row_;
    int col = white.end_.col_;
    if (board.squares_[row][col].piece_ != nullptr) {
        if (tolower(board.squares_[row][col].piece_->get_piece()) == 'p') {
            board.halfmoves_ = 0;
        } else {
            board.halfmoves_++;
        }
    } else {
        board.halfmoves_++;
    }
    row = black.end_.row_;
    col = black.end_.col_;
    if (board.squares_[row][col].piece_ != nullptr) {
        if (tolower(board.squares_[row][col].piece_->get_piece()) == 'p') {
            board.halfmoves_ = 0;
        } else {
            board.halfmoves_++;
        }
    } else {
        board.halfmoves_++;
    }
}

bool GameRules::stalemate(Board &board, Player &white, Player &black) {
    Rules rules;
    if (board.white_playes) {
        if (rules.stalemate(board, white)) {
            return true;
        }
    } else {
        if (rules.stalemate(board, black)) {
            return true;
        }
    }
    return false;
}