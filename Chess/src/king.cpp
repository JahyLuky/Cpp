#include "king.h"
#include "board.h"

King::King(char name, char color, const Position &pos)
        : Piece(name, color, pos) {}

King *King::clone() const {
    return new King(*this);
}

// Possible king directions
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

std::vector<Position> *King::possible_moves() {
    // Reset possible moves
    this->moves_.clear();

    int row = this->get_position().row_;
    int col = this->get_position().col_;
    int row_cnt = 0, col_cnt = 0;

    for (const auto &direction: move_directions) {
        row_cnt = row + direction.row_;
        col_cnt = col + direction.col_;

        Position pos(row_cnt, col_cnt);

        add_move(pos);
    }

    // Castling
    if (!this->first_move_) {
        // Queen-side
        Position pos1(this->get_position().row_, this->get_position().col_ - 1);
        Position pos2(this->get_position().row_, this->get_position().col_ - 2);
        add_move(Position(this->get_position().row_, this->get_position().col_ - 2));
        // King-side
        pos1.col_ = this->get_position().col_ + 1;
        pos2.col_ = this->get_position().col_ + 2;
        add_move(Position(this->get_position().row_, this->get_position().col_ + 2));
    }

    return &this->moves_;
}
