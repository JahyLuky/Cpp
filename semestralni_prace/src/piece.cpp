#include "piece.h"
#include "board.h"

// PIECE

Piece::Piece(char name, char color, const Position &pos)
        : type_(name), color_(color), pos_(pos) {}

char Piece::get_piece() const {
    return type_;
}

void Piece::set_piece(char name) {
    type_ = name;
}

char Piece::get_color() const {
    return color_;
}

void Piece::set_color(char color) {
    color_ = color;
}

Position Piece::get_position() const {
    return pos_;
}

void Piece::set_position(const Position &other) {
    pos_.row_ = other.row_;
    pos_.col_ = other.col_;
}

bool Piece::valid_position(const Position &other) const {
    if (other.row_ >= 0 && other.row_ < BOARD_SIZE
        && other.col_ >= 0 && other.col_ < BOARD_SIZE) {
        return true;
    }
    return false;
}

void Piece::add_move(const Position &dest) {
    this->moves_.emplace_back(dest);
}

bool Piece::is_empty(const Board &board, const Position &dest) const {
    if (board.squares_[dest.row_][dest.col_].piece_ == nullptr) {
        return true;
    }
    return false;
}

bool Piece::is_same_color(const Board &board, const Position &dest) const {
    if (this->get_color() == board.squares_[dest.row_][dest.col_].piece_->get_color()) {
        return true;
    }
    return false;
}
