#include "application.h"

bool Application::run() const {
    Menu menu;
    // TODO: input game in menu
    switch (menu.print_menu()) {
        case 't':
            if (!tutorial()) {
                return false;
            }
        case 'a':
            // choose AI
            if (!game('a')) {
                return false;
            }
        case 'h':
            if (!game('h')) {
                return false;
            }
        default:
            return false;
    }

    return true;
}

bool Application::tutorial() const {
    std::cout << "Pravidla:" << std::endl;
    // TODO: set up tutorial
    if (!game('h')) {
        return false;
    }
    return true;
}

// TODO: add maybe into class
// TODO: add vector for captures
void print_captures(const Player &player_white, const Player &player_black) {
    std::cout << "\nW: ";
    for (const auto &item: player_white.captures_) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
    std::cout << "B: ";
    for (const auto &item: player_black.captures_) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
}

void print_color(bool white_plays) {
    if (white_plays) {
        std::cout << "\nWHITE plays!" << std::endl;
    } else {
        std::cout << "\nBLACK plays!" << std::endl;
    }
}

void Application::make_move(Board &board, Player &player) {
    // Starting position of piece
    Square &start = board.squares_[player.start_.row_][player.start_.col_];

    // Final position of piece
    Square &end = board.squares_[player.end_.row_][player.end_.col_];

    bool promote = false;

    // No double move
    if (tolower(start.piece_->get_piece()) == 'p') {
        start.piece_->first_move_ = false;
        if ((player.end_.row_ % 7) == 0) {
            promote = true;
        }
    }

    // No castle
    if (tolower(start.piece_->get_piece()) == 'k'
        || tolower(start.piece_->get_piece()) == 'r') {
        start.piece_->first_move_ = false;
    }

    if (!promote) {
        // Taken piece
        if (end.piece_ != nullptr) {
            player.captures_.push_back(end.piece_->get_piece());
        }
        // Move piece to new position
        end.piece_ = std::unique_ptr<Piece>(start.piece_->clone());
        // Change coor to new position
        end.piece_->set_position(Position(player.end_.row_, player.end_.col_));
    } else {
        // TODO: proper promotion
        end.piece_ = std::make_unique<Queen>('Q', player.color_, Position(player.end_.row_, player.end_.col_));
    }

    // Empty starting square
    start.piece_ = nullptr;
}

bool Application::game(char game_type) const {
    Board board;
    board.init_board();

    std::cout << "\nGame starts!\n" << std::endl;

    // Print classical board
    board.print_color_board();

    HumanPlayer A('W'), B('B');
    bool white_plays = true;

    std::cout << "\nWHITE plays!" << std::endl;

    // TODO: find all possible TIEs
    while (!board.game_over) {
        std::cout << "What is your move?" << std::endl;

        if (std::cin.eof()) {
            std::cout << "End of input or file!" << std::endl;
            break;
        }

        // TODO: based on game_type, change get old and new pos from input or AI

        // Change turns
        if (white_plays) {
            if (A.get_move(board)) {
                make_move(board, A);
                white_plays = false;
            }
        } else {
            if (B.get_move(board)) {
                make_move(board, B);
                white_plays = true;
            }
        }
        print_captures(A, B);

        // Print updated board
        board.print_color_board();
        // Print who plays next
        print_color(white_plays);
    }
    return true;
}