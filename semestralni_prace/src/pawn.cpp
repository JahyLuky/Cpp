#include "pawn.h"

char Pawn::get_piece() const {
    return Piece::get_piece();
}

void Pawn::set_piece(char name) {
    Piece::set_piece(name);
}

void Pawn::print(std::ostream &out, const Piece &item) const {
    if (Pawn::get_square()) {
        out << "\033[40m\033[37m " << get_piece() << " \033[0m";
    } else {
        out << "\033[47m\033[30m " << get_piece() << " \033[0m";
    }
}

char Pawn::get_color() const {
    return Piece::get_color();
}

Position Pawn::get_coor() const {
    return Piece::get_coor();
}

void Pawn::set_coor(Position &other) {
    Piece::set_coor(other);
}

bool Pawn::get_square() const {
    return Piece::get_square();
}

void Pawn::set_square(bool square) {
    Piece::set_square(square);
}

Pawn *Pawn::clone() const {
    return new Pawn(*this);
}
