#include "custom_ai.h"

#include "rules.h"
#include "validate.h"
#include "move.h"

Custom_AI::Custom_AI(char color)
        : Player(color) {}

bool Custom_AI::get_promotion(char &piece) {
    if (color_ == 'W') {
        piece = 'Q';
    } else {
        piece = 'q';
    }
    return true;
}

int Custom_AI::piece_priority(const char pieceType) const{
    switch (tolower(pieceType)) {
        case 'p':
            return 10;
        case 'n':
            return 30;
        case 'b':
            return 30;
        case 'r':
            return 50;
        case 'q':
            return 90;
        default:
            return 10;
    }
}

void Custom_AI::possible_pieces_and_moves(Board &board) {
    Rules rules;
    Validate validate;
    Move move;
    bool piece_taken = false, checked = false;
    if (rules.checked(board, king_, color_)) {
        checked = true;
    }
    Position best_pos_start{}, best_pos_end{};
    int best_move = 0;
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
            char piece = board.squares_[start.row_][start.col_].piece_->get_piece();
            if (piece_priority(piece) > best_move) {
                best_pos_start = start;
                best_pos_end = end;
                best_move = piece_priority(piece);
            }
            if (piece_taken) {
                return;
            }
        }
    }
    start_ = best_pos_start;
    end_ = best_pos_end;
}

bool Custom_AI::get_move(Board &board) {
    possible_pieces_and_moves(board);
    return true;
}
