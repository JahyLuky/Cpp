#include "position.h"

Position::Position(int a, int b)
        : row_(a), col_(b) {}

Position::Position(const Position &other)
        : row_(other.row_), col_(other.col_) {}

Position &Position::operator=(const Position &other) {
    if (this != &other) {
        row_ = other.row_;
        col_ = other.col_;
    }
    return *this;
}

bool Position::operator==(const Position &other) const {
    if (this->row_ == other.row_
        && this->col_ == other.col_) {
        return true;
    }
    return false;
}

bool Position::operator!=(const Position &other) const {
    if (*this == other) {
        return false;
    }
    return true;
}
