#include "knight.h"
#include "board.h"

Knight *Knight::clone() const {
    return new Knight(*this);
}

// Possible knight directions
const std::vector<Position> move_directions = {
        Position(-2, -1), Position(-2, 1),
        Position(-1, -2), Position(-1, 2),
        Position(1, -2), Position(1, 2),
        Position(2, -1), Position(2, 1)
};

std::vector<Position> *Knight::possible_moves(Board &board) {
    // reset possible moves
    this->moves_.clear();

    int row = this->get_position().row_;
    int col = this->get_position().col_;
    int next_row = 0, next_col = 0;

    for (const auto &move: move_directions) {
        next_row = row + move.row_;
        next_col = col + move.col_;

        Position pos(next_row, next_col);

        if(valid_position(pos)) {
            if (is_empty(board, pos)
            || !is_same_color(board, pos)) {
                add_move(pos);
            }
        }
    }

    return &this->moves_;
}
