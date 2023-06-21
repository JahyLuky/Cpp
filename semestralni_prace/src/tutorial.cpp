#include "tutorial.h"

#include <stdexcept>

void Tutorial::print_menu() const {
    std::cout << "------MENU------\nChoose your rule of game to try:\n";
    std::cout << "(c):\tCheck" << std::endl;
    std::cout << "(m):\tCheckmate" << std::endl;
    std::cout << "(t):\tCastling" << std::endl;
    std::cout << "(r):\tPromotion" << std::endl;
    std::cout << "(p):\tEn passant" << std::endl;
    std::cout << "(a):\tPat" << std::endl;
    std::cout << "(e):\tEnd tutorial" << std::endl;
}

bool Tutorial::start_tutorial(Board &board, const std::string &file_path) {
    Application app;
    File_manager manager;
    HumanPlayer white('W');
    HumanPlayer black('B');
    try {
        manager.open_file(file_path, board, white, black);
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    white.fill_pieces(board);
    black.fill_pieces(board);
    app.game(board, white, black, true);
    return true;
}

void Tutorial::run_tutorial() {
    std::cout << "Tutorial for chess\nChoose what you want to try playing.\n" << std::endl;
    print_menu();
    Board board;
    std::string file_path;
    char in;
    std::cin >> in;
    int input_counter = 0;
    if (std::cin.eof()) {
        throw std::runtime_error("End of input");
    }
    while (true) {
        if (in == 'e') {
            std::cout << "End of tutorial!" << std::endl;
            break;
        }
        // 10 wrong inputs = end program
        if (input_counter == 10) {
            std::cout << "You have entered 10 times in a row wrong input!" << std::endl;
            break;
        }
        input_counter++;
        switch (in) {
            case 'c': // Check
                file_path = "examples/tutorial/check.txt";
                std::cout << "Block the check. (c2 c3), (b1 c3), ..." << std::endl;
                start_tutorial(board, file_path);
                board.reset_board();
                print_menu();
                break;
            case 'm': // Checkmate
                file_path = "examples/tutorial/checkmate.txt";
                std::cout << "Checkmate black. (f3 f7)" << std::endl;
                start_tutorial(board, file_path);
                if (board.squares_[1][5].piece_ != nullptr
                    && board.squares_[1][5].piece_->get_piece() == 'Q') {
                    std::cout << "CHECKMATE!" << std::endl;
                } else {
                    std::cout << "That is not checkmate!" << std::endl;
                }
                board.reset_board();
                print_menu();
                break;
            case 't': // Castling
                file_path = "examples/tutorial/castling.txt";
                std::cout << "Do the castling. (e1 g1)" << std::endl;
                start_tutorial(board, file_path);
                if (board.squares_[7][5].piece_ != nullptr
                    && board.squares_[7][5].piece_->get_piece() == 'R') {
                    std::cout << "Good job!" << std::endl;
                } else {
                    std::cout << "That is not castling!" << std::endl;
                }
                board.reset_board();
                print_menu();
                break;
            case 'r': // Promotion
                file_path = "examples/tutorial/promotion.txt";
                std::cout << "Promote. (d7 d8)" << std::endl;
                start_tutorial(board, file_path);
                if (board.squares_[0][3].piece_ != nullptr) {
                    std::cout << "Good job!" << std::endl;
                } else {
                    std::cout << "That is not promotion!" << std::endl;
                }
                board.reset_board();
                print_menu();
                break;
            case 'p': // En passant
                file_path = "examples/tutorial/en_passant.txt";
                std::cout << "Do en passant. (e5 d6)" << std::endl;
                start_tutorial(board, file_path);
                if (board.squares_[2][3].piece_ != nullptr) {
                    std::cout << "Good job!" << std::endl;
                } else {
                    std::cout << "That is not en passant!" << std::endl;
                }
                board.reset_board();
                print_menu();
                break;
            case 'a': // Pat
                file_path = "examples/tutorial/pat.txt";
                std::cout << "Pat your opponent. (e2 f2)" << std::endl;
                start_tutorial(board, file_path);
                board.reset_board();
                print_menu();
                break;
            default:
                std::cout << "Wrong input!" << std::endl;
        }
        std::cin >> in;
        if (std::cin.eof()) {
            throw std::runtime_error("End of menu input");
        }
    }
    std::cout << std::endl;
}
