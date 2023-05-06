#include "piece.h"

/**
 * Represents PAWN
 */
class Pawn : public Piece {
public:
    Pawn(char name, char color,const Position& pos)
            : Piece(name, color, pos) {}

    char get_piece() const;

    void set_piece(char name) override;

    void print(std::ostream &out, const Piece &item) const override;

    char get_color() const;

    Position get_coor() const;

    void set_coor(Position &other) override;

    bool get_square() const;

    void set_square(bool square) override;

    Pawn *clone() const override;

private:

};