#include "custom_ai2.h"

#include "rules.h"
#include "validate.h"
#include "move.h"

Custom_AI2::Custom_AI2(char color)
        : Player(color) {}

bool Custom_AI2::get_promotion(char &piece) {
    if (color_ == 'W') {
        piece = 'Q';
    } else {
        piece = 'q';
    }
    return true;
}

void Custom_AI2::possible_pieces_and_moves(Board &board) {
    Rules rules;
    Move move;
    Validate validate;
    bool piece_taken = false;
    bool checked = false;
    if (rules.checked(board, king_, color_)) {
        checked = true;
    }
    // Iterate through each possible start position and evaluate the resulting board state
    for (const Position &start: pieces_pos_) {
        std::vector<Position> possibleEndPositions;
        possibleEndPositions.clear();
        Square &square = board.squares_[start.row_][start.col_];
        for (const auto &moves: *square.piece_->possible_moves()) {
            if (!validate.valid_position(moves)
                || moves == start) {
                continue;
            }
            Position start_save = start_;
            Position end_save = end_;
            start_ = start;
            end_ = moves;
            if (!validate.validate_move(board, *this, false)) {
                start_ = start_save;
                end_ = end_save;
                continue;
            }
            start_ = start_save;
            end_ = end_save;
            possibleEndPositions.emplace_back(moves);
        }
        Position king(king_);
        bool castle = castling_;
        bool en_passant = en_passant_;
        std::vector<Position> pieces_pos_save = pieces_pos_;
        // Iterate through each possible end position for the current start position
        for (const Position &end: possibleEndPositions) {
            // Make the move on the board
            start_ = start;
            end_ = end;
            if (board.squares_[end_.row_][end_.col_].piece_ != nullptr) {
                piece_taken = true;
            }
            if (checked) {
                move.make_move(board, *this);
                if (!rules.checked(board, king_, color_)) {
                    checked = false;
                }
                castling_ = castle;
                en_passant_ = en_passant;
                king_ = king;
                pieces_pos_ = pieces_pos_save;
                // Undo the move
                move.undo_move(board, *this);
                if (!checked) {
                    return;
                }
                continue;
            }
            if (piece_taken) {
                return;
            }
            if ((end.row_ == 3 || end.row_ == 4)
                && (end.col_ == 3 || end.col_ == 4)) {
                return;
            }
        }
    }
}

bool Custom_AI2::get_move(Board &board) {
    possible_pieces_and_moves(board);
    return true;
}
