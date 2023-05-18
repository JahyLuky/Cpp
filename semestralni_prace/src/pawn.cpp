#include "pawn.h"

Pawn *Pawn::clone() const {
    return new Pawn(*this);
}

std::vector<Position>* Pawn::possible_moves() {
    // reset possible moves
    moves_.clear();

    int row = this->get_coor().row_;
    int col = this->get_coor().col_;

    // TODO: moves out of border (8 * 8)
    // distinguish piece color
    if (this->get_color() == 'W') {
        moves_.emplace_back((row - 1), col);
        if (this->first_move) {
            moves_.emplace_back((row - 2), col);
        }
    } else {
        moves_.emplace_back((row + 1), col);
        if (this->first_move) {
            moves_.emplace_back((row + 2), col);
        }
    }

    return &moves_;
}