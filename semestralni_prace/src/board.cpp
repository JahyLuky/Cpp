#include "board.h"

Board::Board() {
    white_playes = true;
    game_over = game_not_finished;
    squares_.clear();
    taken_.piece_ = nullptr;
    start_.piece_ = nullptr;
    end_.piece_ = nullptr;
    halfmoves_ = 0;
    fullmoves_ = 0;
}

// Set classical chess board with all pieces
const Square startingPieces[] = {
        //-----BLACK-----
        {Position(0, 0), std::make_unique<Rook>('r', 'B', Position(0, 0))},
        {Position(0, 1), std::make_unique<Knight>('n', 'B', Position(0, 1))},
        {Position(0, 2), std::make_unique<Bishop>('b', 'B', Position(0, 2))},
        {Position(0, 3), std::make_unique<Queen>('q', 'B', Position(0, 3))},
        {Position(0, 4), std::make_unique<King>('k', 'B', Position(0, 4))},
        {Position(0, 5), std::make_unique<Bishop>('b', 'B', Position(0, 5))},
        {Position(0, 6), std::make_unique<Knight>('n', 'B', Position(0, 6))},
        {Position(0, 7), std::make_unique<Pawn>('r', 'B', Position(0, 7))},
        {Position(1, 0), std::make_unique<Pawn>('p', 'B', Position(1, 0))},
        {Position(1, 1), std::make_unique<Pawn>('p', 'B', Position(1, 1))},
        {Position(1, 2), std::make_unique<Pawn>('p', 'B', Position(1, 2))},
        {Position(1, 3), std::make_unique<Pawn>('p', 'B', Position(1, 3))},
        {Position(1, 4), std::make_unique<Pawn>('p', 'B', Position(1, 4))},
        {Position(1, 5), std::make_unique<Pawn>('p', 'B', Position(1, 5))},
        {Position(1, 6), std::make_unique<Pawn>('p', 'B', Position(1, 6))},
        {Position(1, 7), std::make_unique<Pawn>('p', 'B', Position(1, 7))},
        //-----WHITE-----
        {Position(6, 0), std::make_unique<Pawn>('P', 'W', Position(6, 0))},
        {Position(6, 1), std::make_unique<Pawn>('P', 'W', Position(6, 1))},
        {Position(6, 2), std::make_unique<Pawn>('P', 'W', Position(6, 2))},
        {Position(6, 3), std::make_unique<Pawn>('P', 'W', Position(6, 3))},
        {Position(6, 4), std::make_unique<Pawn>('P', 'W', Position(6, 4))},
        {Position(6, 5), std::make_unique<Pawn>('P', 'W', Position(6, 5))},
        {Position(6, 6), std::make_unique<Pawn>('P', 'W', Position(6, 6))},
        {Position(6, 7), std::make_unique<Pawn>('P', 'W', Position(6, 7))},
        {Position(7, 0), std::make_unique<Pawn>('R', 'W', Position(7, 0))},
        {Position(7, 1), std::make_unique<Knight>('N', 'W', Position(7, 1))},
        {Position(7, 2), std::make_unique<Bishop>('B', 'W', Position(7, 2))},
        {Position(7, 3), std::make_unique<Queen>('Q', 'W', Position(7, 3))},
        {Position(7, 4), std::make_unique<King>('K', 'W', Position(7, 4))},
        {Position(7, 5), std::make_unique<Bishop>('B', 'W', Position(7, 5))},
        {Position(7, 6), std::make_unique<Knight>('N', 'W', Position(7, 6))},
        {Position(7, 7), std::make_unique<Rook>('R', 'W', Position(7, 7))},
};

void Board::init_board() {
    start_.piece_ = nullptr;
    start_.pos_.row_ = 0;
    start_.pos_.col_ = 0;
    end_.piece_ = nullptr;
    end_.pos_.row_ = 0;
    end_.pos_.col_ = 0;
    taken_.piece_ = nullptr;
    taken_.pos_.row_ = 0;
    taken_.pos_.col_ = 0;
    game_over = game_not_finished;
    squares_.clear();
    halfmoves_ = 0;
    fullmoves_ = 0;
    white_playes = true;
    int piece_cnt = 0;
    std::vector<Square> pieces;
    for (int row = 0; row < 8; ++row) {
        pieces.clear();
        for (int col = 0; col < 8; ++col) {
            // fill 1, 2, 7, 8 rows with chess pieces
            if (row == 0 || row == 1 || row == 6 || row == 7) {
                pieces.emplace_back(std::move(startingPieces[piece_cnt++]));
            } else {
                // fill rest of the board with empty squares
                pieces.emplace_back(Position(row, col), nullptr);
            }
        }
        this->squares_.emplace_back(std::move(pieces));
    }
}

void Board::print_board() const {
    // Indicates if a square is black or white
    bool isBlackSquare = false;
    // Row coordinates
    int number_coordinates = 8;
    // Column coordinates
    int coordinates_itr = 0;
    std::vector<char> word_coordinates = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    // Print top side coordinates
    while (coordinates_itr < 8) {
        if (coordinates_itr == 0) {
            std::cout << "   " << word_coordinates[coordinates_itr];
        } else {
            std::cout << "  " << word_coordinates[coordinates_itr];
        }
        coordinates_itr++;
    }
    std::cout << std::endl;
    int row_pos = 0, col_pos = 0;
    for (const auto &row: this->squares_) {
        // Print left side coordinates
        std::cout << number_coordinates << " ";
        for (const auto &col: row) {
            isBlackSquare = (row_pos + col_pos) % 2 == 1;
            /** Print the square
             * "\033[" is the escape code for the ANSI escape sequence
             * "40" is the code for the background color, in this case black
             * "37" is the code for the foreground color, in this case white
             * "m" is the code to terminate the color sequence
             * "\033[0m" resets the text color to the default
             */
            if (isBlackSquare) {
                if (col.piece_ != nullptr) {
                    std::cout << "\033[40m\033[37m " << col.piece_->get_piece() << " \033[0m";
                } else {
                    std::cout << "\033[40m\033[37m " << ' ' << " \033[0m";
                }
            } else {
                if (col.piece_ != nullptr) {
                    std::cout << "\033[47m\033[30m " << col.piece_->get_piece() << " \033[0m";
                } else {
                    std::cout << "\033[47m\033[30m " << ' ' << " \033[0m";
                }
            }
            col_pos++;
        }
        // Print right side coordinates
        std::cout << " " << number_coordinates;
        std::cout << std::endl;
        number_coordinates--;
        row_pos++;
    }
    // Print bottom side coordinates
    coordinates_itr = 0;
    while (coordinates_itr < 8) {
        if (coordinates_itr == 0) {
            std::cout << "   " << word_coordinates[coordinates_itr];
        } else {
            std::cout << "  " << word_coordinates[coordinates_itr];
        }
        coordinates_itr++;
    }
    std::cout << std::endl;
}

void Board::reset_board() {
    game_over = game_not_finished;
    squares_.clear();
    halfmoves_ = 0;
    fullmoves_ = 0;
    white_playes = true;
    start_.piece_ = nullptr;
    start_.pos_.row_ = 0;
    start_.pos_.col_ = 0;
    end_.piece_ = nullptr;
    end_.pos_.row_ = 0;
    end_.pos_.col_ = 0;
    taken_.piece_ = nullptr;
    taken_.pos_.row_ = 0;
    taken_.pos_.col_ = 0;
}
