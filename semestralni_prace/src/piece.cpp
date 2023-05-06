#include "piece.h"

// output operators
std::ostream &operator<<(std::ostream &out, const Piece &item) {
    item.print(out, item);
    return out;
}