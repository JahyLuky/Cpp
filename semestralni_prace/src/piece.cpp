#include "piece.h"

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

Position Piece::get_coor() const {
    return pos_;
}

void Piece::set_coor(Position &other) {
    pos_.row_ = other.row_;
    pos_.col_ = other.col_;
}
