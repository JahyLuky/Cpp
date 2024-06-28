#include "move.h"

#include <stdexcept>

void Move::move_piece(Square &start, Square &end) {
    // Move piece to new position
    end.piece_ = std::unique_ptr<Piece>(start.piece_->clone());

    // Change coor to new position
    end.piece_->set_position(Position(end.pos_.row_, end.pos_.col_));

    // Empty starting square
    start.piece_ = nullptr;
}

void Move::handle_castling(Board &board, Player &player, Square &start, Square &end) {
    // Move king
    move_piece(start, end);

    // 3 -> left
    int side = 3;
    // first col
    int rook_col = 0;
    if (end.pos_.col_ == 6) {
        // -2 -> right
        side = -2;
        // last col
        rook_col = 7;
    }

    Square &rook_start = board.squares_[player.end_.row_][rook_col];
    Square &rook_end = board.squares_[player.end_.row_][rook_col + side];

    move_piece(rook_start, rook_end);

    player.castling_ = false;
}

void Move::promotion(Board &board, Player &player, Square &start, Square &end, char piece) {
    if (tolower(piece) == 'q') {
        if (player.color_ == 'W') {
            piece = 'Q';
        } else {
            piece = 'q';
        }
        start.piece_ = std::make_unique<Queen>(piece, player.color_, Position(player.end_.row_, player.end_.col_));
    } else if (tolower(piece) == 'b') {
        if (player.color_ == 'W') {
            piece = 'B';
        } else {
            piece = 'b';
        }
        start.piece_ = std::make_unique<Bishop>(piece, player.color_, Position(player.end_.row_, player.end_.col_));
    } else if (tolower(piece) == 'n') {
        if (player.color_ == 'W') {
            piece = 'N';
        } else {
            piece = 'n';
        }
        start.piece_ = std::make_unique<Knight>(piece, player.color_, Position(player.end_.row_, player.end_.col_));
    } else if (tolower(piece) == 'r') {
        if (player.color_ == 'W') {
            piece = 'R';
        } else {
            piece = 'r';
        }
        start.piece_ = std::make_unique<Rook>(piece, player.color_, Position(player.end_.row_, player.end_.col_));
    }

    player.update_pieces(start.pos_, end.pos_);

    if (end.piece_ != nullptr) {
        board.taken_.pos_ = end.pos_;
        move_piece(end, board.taken_);
    }
    move_piece(start, end);
}

void Move::make_move(Board &board, Player &player) {
    // Starting position of piece
    Square &start = board.squares_[player.start_.row_][player.start_.col_];

    // Final position of piece
    Square &end = board.squares_[player.end_.row_][player.end_.col_];

    bool promote = false;

    // No double move
    if (tolower(start.piece_->get_piece()) == 'p') {
        start.piece_->first_move_ = true;
        if ((player.end_.row_ % 7) == 0) {
            promote = true;
        }
    }

    // TODO: piece in a way or check
    // Castling
    if (player.castling_) {
        handle_castling(board, player, start, end);
        return;
    }

    // King's first move
    if (tolower(start.piece_->get_piece()) == 'k') {
        start.piece_->first_move_ = true;
        player.king_ = end.pos_;
    }

    // Rook's first move
    if (tolower(start.piece_->get_piece()) == 'r') {
        start.piece_->first_move_ = true;
    }

    if (!promote) {
        player.update_pieces(start.pos_, end.pos_);
        if (end.piece_ != nullptr) {
            board.taken_.pos_ = end.pos_;
            move_piece(end, board.taken_);
        }
        move_piece(start, end);
    } else {
        char piece;
        int wrong_input_cnt = 0;
        while (!player.get_promotion(piece)) {
            wrong_input_cnt++;
            if (wrong_input_cnt == 10) {
                throw std::runtime_error("You entered wrong piece 10 times in a row!");
            }
        }
        promotion(board, player, start, end, piece);
    }
}

void Move::undo_move(Board &board, Player &player) {
    // Get the start and end positions of the move
    const Position &start = player.start_;
    const Position &end = player.end_;

    // Get the squares at the start and end positions
    Square &startSquare = board.squares_[start.row_][start.col_];
    Square &endSquare = board.squares_[end.row_][end.col_];

    move_piece(endSquare, startSquare);

    if (tolower(startSquare.piece_->get_piece()) == 'p') {
        if (startSquare.pos_.row_ == 1
            && player.color_ == 'B') {
            startSquare.piece_->first_move_ = false;
        }
        if (startSquare.pos_.row_ == 6
            && player.color_ == 'W') {
            startSquare.piece_->first_move_ = false;
        }
    }

    // Move the piece back to the start position
    if (board.taken_.piece_ != nullptr) {
        // Store the moved piece
        board.taken_.pos_ = endSquare.pos_;
        move_piece(board.taken_, endSquare);
        board.taken_.piece_ = nullptr;
    }

    char piece = 'p';
    if (player.color_ == 'W') {
        piece = 'P';
    }

    bool promote = false;
    // No double move
    if (tolower(startSquare.piece_->get_piece()) == 'p') {
        if ((player.end_.row_ % 7) == 0) {
            promote = true;
        }
    }
    // If it was a pawn promotion, undo the promotion
    if (promote) {
        startSquare.piece_ = std::make_unique<Pawn>(piece, player.color_,
                                                    Position(player.end_.row_, player.end_.col_));
        startSquare.piece_->first_move_ = true;
        return;
    }
    // If it was a castling move, undo the rook move
    if (player.castling_) {
        if (end.col_ == 2) {
            // Undo left castling
            Square &rookSquare = board.squares_[start.row_][0];
            Square &newRookSquare = board.squares_[start.row_][3];
            move_piece(newRookSquare, rookSquare);
            newRookSquare.piece_ = nullptr;
            rookSquare.piece_->set_position(Position(start.row_, 0));
            rookSquare.piece_->first_move_ = false;
        } else if (end.col_ == 6) {
            // Undo right castling
            Square &rookSquare = board.squares_[start.row_][7];
            Square &newRookSquare = board.squares_[start.row_][5];
            move_piece(newRookSquare, rookSquare);
            newRookSquare.piece_ = nullptr;
            rookSquare.piece_->set_position(Position(start.row_, 7));
            rookSquare.piece_->first_move_ = false;
        }
        player.castling_ = false;
        return;
    }

    // If it was an en passant move, restore the captured pawn
    if (player.en_passant_) {
        Square &capturedSquare = board.squares_[start.row_][end.col_];
        capturedSquare.piece_ = std::make_unique<Pawn>(piece, player.color_,
                                                       Position(player.end_.row_, player.end_.col_));
        capturedSquare.piece_->set_position(Position(start.row_, end.col_));
        player.en_passant_ = false;
    }
}
