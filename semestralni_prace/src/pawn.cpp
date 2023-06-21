#include "pawn.h"

Pawn::Pawn(char name, char color, const Position &pos)
        : Piece(name, color, pos) {}

Pawn *Pawn::clone() const {
    return new Pawn(*this);
}

std::vector<Position> *Pawn::possible_moves() {
    // reset possible moves
    this->moves_.clear();

    int row = this->get_position().row_;
    int col = this->get_position().col_;

    // distinguish piece color
    if (this->get_color() == 'W') {
        if ((row - 1) >= 0) {
            // Move to left side
            if ((col - 1) >= 0) {
                add_move(Position((row - 1), (col - 1)));
            }
            // Move to right side
            if ((col + 1) < 8) {
                add_move(Position((row - 1), (col + 1)));
            }
            // Move forward
            add_move(Position((row - 1), (col)));
            if (!this->first_move_) {
                add_move(Position((row - 2), (col)));
            }

        }
    } else {
        if ((row + 1) < 8) {
            // Move to left side
            if ((col - 1) >= 0) {
                add_move(Position((row + 1), (col - 1)));
            }
            // Move to right side
            if ((col + 1) < 8) {
                add_move(Position((row + 1), (col + 1)));
            }
            // Move forward
            add_move(Position((row + 1), (col)));
            if (!this->first_move_) {
                add_move(Position((row + 2), (col)));
            }
        }
    }

    return &this->moves_;
}
