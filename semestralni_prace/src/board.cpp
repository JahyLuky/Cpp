#include "board.h"
#include "piece.h"
#include "pawn.h"


// set classical chess board with all pieces
Square startingPieces[] = {
        // TODO: change pawn
        //-----BLACK-----
        {Position(0, 0), std::make_unique<Pawn>('R', 'B', Position(0, 0))},
        {Position(0, 1), std::make_unique<Pawn>('N', 'B', Position(0, 1))},
        {Position(0, 2), std::make_unique<Pawn>('B', 'B', Position(0, 2))},
        {Position(0, 3), std::make_unique<Pawn>('Q', 'B', Position(0, 3))},
        {Position(0, 4), std::make_unique<Pawn>('K', 'B', Position(0, 4))},
        {Position(0, 5), std::make_unique<Pawn>('B', 'B', Position(0, 5))},
        {Position(0, 6), std::make_unique<Pawn>('N', 'B', Position(0, 6))},
        {Position(0, 7), std::make_unique<Pawn>('R', 'B', Position(0, 7))},
        {Position(1, 0), std::make_unique<Pawn>('P', 'B', Position(1, 0))},
        {Position(1, 1), std::make_unique<Pawn>('P', 'B', Position(1, 1))},
        {Position(1, 2), std::make_unique<Pawn>('P', 'B', Position(1, 2))},
        {Position(1, 3), std::make_unique<Pawn>('P', 'B', Position(1, 3))},
        {Position(1, 4), std::make_unique<Pawn>('P', 'B', Position(1, 4))},
        {Position(1, 5), std::make_unique<Pawn>('P', 'B', Position(1, 5))},
        {Position(1, 6), std::make_unique<Pawn>('P', 'B', Position(1, 6))},
        {Position(1, 7), std::make_unique<Pawn>('P', 'B', Position(1, 7))},
        //-----WHITE-----
        {Position(6, 0), std::make_unique<Pawn>('p', 'W', Position(6, 0))},
        {Position(6, 1), std::make_unique<Pawn>('p', 'W', Position(6, 1))},
        {Position(6, 2), std::make_unique<Pawn>('p', 'W', Position(6, 2))},
        {Position(6, 3), std::make_unique<Pawn>('p', 'W', Position(6, 3))},
        {Position(6, 4), std::make_unique<Pawn>('p', 'W', Position(6, 4))},
        {Position(6, 5), std::make_unique<Pawn>('p', 'W', Position(6, 5))},
        {Position(6, 6), std::make_unique<Pawn>('p', 'W', Position(6, 6))},
        {Position(6, 7), std::make_unique<Pawn>('p', 'W', Position(6, 7))},
        {Position(7, 0), std::make_unique<Pawn>('r', 'W', Position(7, 0))},
        {Position(7, 1), std::make_unique<Pawn>('n', 'W', Position(7, 1))},
        {Position(7, 2), std::make_unique<Pawn>('b', 'W', Position(7, 2))},
        {Position(7, 3), std::make_unique<Pawn>('q', 'W', Position(7, 3))},
        {Position(7, 4), std::make_unique<Pawn>('k', 'W', Position(7, 4))},
        {Position(7, 5), std::make_unique<Pawn>('b', 'W', Position(7, 5))},
        {Position(7, 6), std::make_unique<Pawn>('n', 'W', Position(7, 6))},
        {Position(7, 7), std::make_unique<Pawn>('r', 'W', Position(7, 7))},
        {Position(7, 7), std::make_unique<Pawn>('r', 'W', Position(7, 7))},
};

void Board::init_board() {
    int piece_cnt = 0;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (piece_cnt < 16) {
                // BLACK pieces
                this->squares_.emplace_back(std::move(startingPieces[piece_cnt]));
            } else if (piece_cnt > 47) {
                // WHITE pieces (32 empty squares)
                this->squares_.emplace_back(std::move(startingPieces[piece_cnt - 32]));
            } else {
                // Empty squares
                this->squares_.emplace_back(Position(row, col), nullptr);
            }
            piece_cnt++;
        }
    }
}

void Board::print_color_board() const {
    // Indicates if a square is black or white
    bool isBlackSquare = false;
    // Row coordinates
    int number_coordinates = BOARD_SIZE;
    // Column coordinates
    int coordinates_itr = 0;
    std::vector<char> word_coordinates = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    // Print top side coordinates
    while (coordinates_itr < BOARD_SIZE) {
        if (coordinates_itr == 0) {
            std::cout << "   " << word_coordinates[coordinates_itr];
        } else {
            std::cout << "  " << word_coordinates[coordinates_itr];
        }
        coordinates_itr++;
    }
    std::cout << std::endl;

    int board_itr = 0;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        // Print left side coordinates
        std::cout << number_coordinates << " ";
        for (int col = 0; col < BOARD_SIZE; ++col, ++board_itr) {
            isBlackSquare = (row + col) % 2 == 1;

            /** Print the square
             * "\033[" is the escape code for the ANSI escape sequence
             * "40" is the code for the background color, in this case black
             * "37" is the code for the foreground color, in this case white
             * "m" is the code to terminate the color sequence
             * "\033[0m" resets the text color to the default
             */
            if (isBlackSquare) {
                if (this->squares_[board_itr].piece_ != nullptr) {
                    std::cout << "\033[40m\033[37m " << this->squares_[board_itr].piece_->get_piece() << " \033[0m";
                } else {
                    std::cout << "\033[40m\033[37m " << ' ' << " \033[0m";
                }
            } else {
                if (this->squares_[board_itr].piece_ != nullptr) {
                    std::cout << "\033[47m\033[30m " << this->squares_[board_itr].piece_->get_piece() << " \033[0m";
                } else {
                    std::cout << "\033[47m\033[30m " << ' ' << " \033[0m";
                }
            }

        }
        // Print right side coordinates
        std::cout << " " << number_coordinates;
        std::cout << std::endl;
        number_coordinates--;
    }

    // Print bottom side coordinates
    coordinates_itr = 0;
    while (coordinates_itr < BOARD_SIZE) {
        if (coordinates_itr == 0) {
            std::cout << "   " << word_coordinates[coordinates_itr];
        } else {
            std::cout << "  " << word_coordinates[coordinates_itr];
        }
        coordinates_itr++;
    }
    std::cout << std::endl;
}

void Board::print_basic_board() const {
    // Indicates if a square is black or white
    bool isBlackSquare = false;
    // Row coordinates
    int number_coordinates = BOARD_SIZE;
    // Column coordinates
    int coordinates_itr = 0;
    std::vector<char> word_coordinates = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    // Print top side coordinates
    while (coordinates_itr < BOARD_SIZE) {
        if (coordinates_itr == 0) {
            std::cout << "   " << word_coordinates[coordinates_itr];
        } else {
            std::cout << "  " << word_coordinates[coordinates_itr];
        }
        coordinates_itr++;
    }
    std::cout << std::endl;
    std::cout << "  ________________________  " << std::endl;

    int board_itr = 0;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        // Print left side coordinates
        std::cout << number_coordinates << "|";
        for (int col = 0; col < BOARD_SIZE; ++col, ++board_itr) {
            isBlackSquare = (row + col) % 2 == 1;

            if (isBlackSquare) {
                if (this->squares_[board_itr].piece_ != nullptr) {
                    std::cout << " " << this->squares_[board_itr].piece_->get_piece() << " ";
                } else {
                    std::cout << " " << ' ' << " ";
                }
            } else {
                if (this->squares_[board_itr].piece_ != nullptr) {
                    std::cout << " " << this->squares_[board_itr].piece_->get_piece() << " ";
                } else {
                    std::cout << " " << ' ' << " ";
                }
            }
        }
        // Print right side coordinates
        std::cout << "|" << number_coordinates;
        std::cout << std::endl;
        number_coordinates--;
    }

    // Print bottom side coordinates
    std::cout << "  ------------------------  " << std::endl;
    coordinates_itr = 0;
    while (coordinates_itr < BOARD_SIZE) {
        if (coordinates_itr == 0) {
            std::cout << "   " << word_coordinates[coordinates_itr];
        } else {
            std::cout << "  " << word_coordinates[coordinates_itr];
        }
        coordinates_itr++;
    }
    std::cout << std::endl;
}

