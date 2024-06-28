#include "minimax.h"

#include "rules.h"
#include "validate.h"

Minimax::Minimax(char color)
        : Player(color), depth_(1) {}

bool Minimax::get_promotion(char &piece) {
    if (color_ == 'W') {
        piece = 'Q';
    } else {
        piece = 'q';
    }
    return true;
}

int Minimax::get_piece_value(const char pieceType) {
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
            return 0;
    }
}

int Minimax::evaluate_board(const Board &board) {
    int whiteMaterial = 0;
    int blackMaterial = 0;
    // Iterate over the board and count the material for each player
    for (const auto &rows: board.squares_) {
        for (const auto &cols: rows) {
            Square square = board.squares_[cols.pos_.row_][cols.pos_.col_];
            if (square.piece_ == nullptr) {
                continue;
            }
            if (square.piece_->get_color() == 'W') {
                whiteMaterial += get_piece_value(square.piece_->get_piece());
            } else {
                blackMaterial += get_piece_value(square.piece_->get_piece());
            }
        }
    }
    // Calculate the score based on the material difference
    int score = whiteMaterial - blackMaterial;
    return score;
}

int Minimax::minimax_algorithm(Board &board, int depth, bool maximizingPlayer, Move &move) {
    Rules rules;
    Validate validate;
    if (depth == 0) {
        // Reached the maximum depth, evaluate the current board state
        return evaluate_board(board);
    }
    bool checked = false;
    if (rules.checked(board, king_, color_)) {
        checked = true;
    }
    // Initialize the best score based on the current player
    int best_score = maximizingPlayer ? -666 : 666;
    // Iterate through each possible start position and evaluate the resulting board state
    for (const Position &start: pieces_pos_) {
        std::vector<Position> possibleEndPositions;
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
            move.make_move(board, *this);

            if (checked) {
                if (rules.checked(board, king_, color_)) {
                    castling_ = castle;
                    en_passant_ = en_passant;
                    king_ = king;
                    pieces_pos_ = pieces_pos_save;
                    // Undo the move
                    move.undo_move(board, *this);
                    continue;
                } else {
                    castling_ = castle;
                    en_passant_ = en_passant;
                    king_ = king;
                    pieces_pos_ = pieces_pos_save;
                    // Undo the move
                    move.undo_move(board, *this);
                    best_score = maximizingPlayer ? -666 : 666;
                    return best_score;
                }
            }
            // Recursively call the minimax algorithm for the opponent's turn
            int score = minimax_algorithm(board, depth - 1, !maximizingPlayer, move);
            castling_ = castle;
            en_passant_ = en_passant;
            king_ = king;
            pieces_pos_ = pieces_pos_save;
            // Undo the move
            move.undo_move(board, *this);
            // Update the best score and best positions based on the current player
            if ((maximizingPlayer && score > best_score)
                || (!maximizingPlayer && score < best_score)) {
                best_score = score;
                start_ = start;
                end_ = end;
            }
        }
    }
    return best_score;
}

bool Minimax::get_move(Board &board) {
    Move move;
    minimax_algorithm(board, depth_, board.white_playes, move);
    return true;
}
