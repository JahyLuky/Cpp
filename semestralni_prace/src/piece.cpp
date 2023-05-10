#include "piece.h"

// PIECE

char Piece::get_piece() const {
    return name_;
}

void Piece::set_piece(char name) {
    name_ = name;
}

char Piece::get_color() const {
    return color_;
}

void Piece::set_color(char color) {
    color_ = color;
}

Position Piece::get_coor() const {
    return pos_;
}

void Piece::set_coor(Position &other) {
    pos_.row_ = other.row_;
    pos_.col_ = other.col_;
}

bool Piece::get_square() const {
    return is_on_black_square;
}

void Piece::set_square(bool square) {
    is_on_black_square = square;
}

std::ostream &operator<<(std::ostream &out, const Piece &item) {
    item.print(out, item);
    return out;
}

