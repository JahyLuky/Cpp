#include "knight.h"

Knight::Knight(char name, char color, const Position &pos)
        : Piece(name, color, pos) {}

Knight *Knight::clone() const {
    return new Knight(*this);
}

// Possible knight directions
const std::vector<Position> move_directions = {
        Position(-2, -1),
        Position(-2, 1),
        Position(-1, -2),
        Position(-1, 2),
        Position(1, -2),
        Position(1, 2),
        Position(2, -1),
        Position(2, 1)
};

std::vector<Position> *Knight::possible_moves() {
    // Reset possible moves
    this->moves_.clear();

    int row = this->get_position().row_;
    int col = this->get_position().col_;
    int row_cnt = 0, col_cnt = 0;

    for (const auto &direction: move_directions) {
        row_cnt = row + direction.row_;
        col_cnt = col + direction.col_;

        Position pos(row_cnt, col_cnt);

        if (valid_position(pos)) {
            add_move(pos);
        }
    }

    return &this->moves_;
}
