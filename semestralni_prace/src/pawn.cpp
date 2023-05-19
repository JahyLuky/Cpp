#include "pawn.h"

Pawn *Pawn::clone() const {
    return new Pawn(*this);
}

std::vector<Position> *Pawn::possible_moves() {
    // reset possible moves
    this->moves_.clear();

    int row = this->get_position().row_;
    int col = this->get_position().col_;

    // TODO: moves out of border (8 * 8)

    // distinguish piece color
    if (this->get_color() == 'W') {
        if ((row - 1) >= 0) {
            // move to side (in make_move validate)
            if ((col - 1) >= 0) {
                //this->moves_.emplace_back((row - 1), (col - 1));
            }
            if ((col + 1) < 8) {
                //this->moves_.emplace_back((row - 1), (col + 1));
            }


            // move forward
            this->moves_.emplace_back((row - 1), col);
            if (this->first_move_) {
                this->moves_.emplace_back((row - 2), col);
            }
        }
    } else {
        if ((row + 1) < 8) {
            // move to side (in make_move validate)
            if ((col - 1) >= 0) {
                //this->moves_.emplace_back((row - 1), (col - 1));
            }
            if ((col + 1) < 8) {
                //this->moves_.emplace_back((row - 1), (col + 1));
            }

            // move forward
            this->moves_.emplace_back((row + 1), col);
            if (this->first_move_) {
                moves_.emplace_back((row + 2), col);
            }
        }
    }

    return &this->moves_;
}