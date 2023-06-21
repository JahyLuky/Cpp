#include "square.h"

Square::Square(const Position &pos)
: pos_(pos){}

Square::Square(const Position &pos, std::unique_ptr<Piece> piece)
        : pos_(pos), piece_(std::move(piece)) {}

Square::Square(const Square &other) {
    this->pos_ = other.pos_;
    if (other.piece_ == nullptr) {
        this->piece_ = nullptr;
    } else {
        this->piece_ = std::unique_ptr<Piece>(other.piece_->clone());
    }
}

Square &Square::operator=(const Square &other) {
    if (this != &other) {
        this->pos_ = other.pos_;
        if (other.piece_ == nullptr) {
            this->piece_ = nullptr;
        } else {
            this->piece_ = std::unique_ptr<Piece>(other.piece_->clone());
        }
    }
    return *this;
}
