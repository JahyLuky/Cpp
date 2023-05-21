#include "pawn.h"
#include "board.h"

Pawn *Pawn::clone() const {
    return new Pawn(*this);
}

std::vector<Position> *Pawn::possible_moves(Board &board) {
    // reset possible moves
    this->moves_.clear();

    int row = this->get_position().row_;
    int col = this->get_position().col_;

    // distinguish piece color
    if (this->get_color() == 'W') {
        if ((row - 1) >= 0) {
            // Move to left side
            if ((col - 1) >= 0
                && !is_empty(board, Position((row - 1), (col - 1)))
                && board.squares_[row - 1][col - 1].piece_->get_color() != this->get_color()) {
                add_move(Position((row - 1), (col - 1)));
            }
            // Move to right side
            if ((col + 1) < 8
                && !is_empty(board, Position((row - 1), (col + 1)))
                && !is_same_color(board, Position((row - 1), (col + 1)))) {
                add_move(Position((row - 1), (col + 1)));
            }
            // move forward
            if (is_empty(board, Position((row - 1), (col)))) {
                add_move(Position((row - 1), (col)));
                if (this->first_move_
                    && board.squares_[row - 2][col].piece_ == nullptr) {
                    add_move(Position((row - 2), (col)));
                }
            }
        }
    } else {
        if ((row + 1) < 8) {
            // Move to left side
            if ((col - 1) >= 0
                && !is_empty(board, Position((row + 1), (col - 1)))
                && board.squares_[row + 1][col - 1].piece_->get_color() != this->get_color()) {
                add_move(Position((row + 1), (col - 1)));
            }
            // Move to right side
            if ((col + 1) < 8
                && !is_empty(board, Position((row + 1), (col + 1)))
                && board.squares_[row + 1][col + 1].piece_->get_color() != this->get_color()) {
                add_move(Position((row + 1), (col + 1)));
            }
            // move forward
            if (is_empty(board, Position((row + 1), (col)))) {
                add_move(Position((row + 1), (col)));
                if (this->first_move_
                    && is_empty(board, Position((row + 2), (col)))) {
                    add_move(Position((row + 2), (col)));
                }
            }
        }
    }

    return &this->moves_;
}

