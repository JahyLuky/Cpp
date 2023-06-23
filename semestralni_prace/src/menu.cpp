#include "menu.h"

#include "minimax.h"
#include "custom_ai.h"
#include "custom_ai2.h"

void Menu::print_menu() const {
    std::cout << "------MENU------\nChoose your type of game:\n";
    std::cout << "(t):\tTutorial" << std::endl;
    std::cout << "(h):\tPlayer vs Player" << std::endl;
    std::cout << "(a):\tPlayer vs AI" << std::endl;
    std::cout << "(l):\tLoad game" << std::endl;
    std::cout << "(e):\tEnd of program" << std::endl;
}

void Menu::start_game() {
    std::cout << "Player vs Player" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "(draw) for draw" << std::endl;
    std::cout << "(resign) for resign" << std::endl;
    std::cout << "(save) for saving your current game" << std::endl;
    Board board;
    HumanPlayer white('W'), black('B');
    Application app;
    board.init_board();
    white.fill_pieces(board);
    black.fill_pieces(board);
    app.game(board, white, black, false);
}

bool Menu::start_AI_game() {
    std::cout << "Player vs AI" << std::endl;
    std::cout << "Choose AI: " << std::endl;
    std::cout << "(m)\t Minimax" << std::endl;
    std::cout << "(a)\t Custom AI" << std::endl;
    std::cout << "(b)\t Custom AI2" << std::endl;
    Board board;
    Application app;
    char in;
    std::cin >> in;
    if (std::cin.eof()) {
        throw std::runtime_error("End of menu input");
    }
    if (in == 'm') {
        std::cout << "As what color do you want to play?" << std::endl;
        std::cout << "(w) for white, (b) for black." << std::endl;
        std::cin >> in;
        if (std::cin.eof()) {
            throw std::runtime_error("End of menu input");
        }
        if (in == 'w') {
            HumanPlayer white('W');
            Minimax minimaxB('B');
            board.init_board();
            white.fill_pieces(board);
            minimaxB.fill_pieces(board);
            app.game(board, white, minimaxB, false);
        } else if (in == 'b') {
            Minimax minimaxW('W');
            HumanPlayer black('B');
            board.init_board();
            minimaxW.fill_pieces(board);
            black.fill_pieces(board);
            app.game(board, minimaxW, black, false);
        } else {
            return false;
        }
    } else if (in == 'a') {
        std::cout << "As what color do you want to play?" << std::endl;
        std::cout << "(w) for white, (b) for black." << std::endl;
        std::cin >> in;
        if (std::cin.eof()) {
            throw std::runtime_error("End of menu input");
        }
        if (in == 'w') {
            HumanPlayer white('W');
            Custom_AI custom_aiB('B');
            board.init_board();
            white.fill_pieces(board);
            custom_aiB.fill_pieces(board);
            app.game(board, white, custom_aiB, false);
        } else if (in == 'b') {
            Custom_AI custom_ai2W('W');
            HumanPlayer black('B');
            board.init_board();
            custom_ai2W.fill_pieces(board);
            black.fill_pieces(board);
            app.game(board, custom_ai2W, black, false);
        } else {
            return false;
        }
    } else if (in == 'b') {
        std::cout << "As what color do you want to play?" << std::endl;
        std::cout << "(w) for white, (b) for black." << std::endl;
        std::cin >> in;
        if (std::cin.eof()) {
            throw std::runtime_error("End of menu input");
        }
        if (in == 'w') {
            HumanPlayer white('W');
            Custom_AI2 custom_aiB('B');
            board.init_board();
            white.fill_pieces(board);
            custom_aiB.fill_pieces(board);
            app.game(board, white, custom_aiB, false);
        } else if (in == 'b') {
            Custom_AI2 custom_ai2W('W');
            HumanPlayer black('B');
            board.init_board();
            custom_ai2W.fill_pieces(board);
            black.fill_pieces(board);
            app.game(board, custom_ai2W, black, false);
        } else {
            return false;
        }
    } else {
        std::cout << "Wrong input, try again!" << std::endl;
        return false;
    }
    return true;
}

bool Menu::load_game() {
    File_manager manager;
    Board board;
    HumanPlayer white('W'), black('B');
    Application app;
    std::string file_path;
    std::string dir_path = "examples/";
    std::set<std::string> dir_file_names;
    manager.list_directory(dir_file_names, dir_path);
    std::cout << "Choose what game you want to open with (.txt):" << std::endl;
    std::cin >> file_path;
    if (std::cin.eof()) {
        throw std::runtime_error("End of menu input");
    }
    if (!manager.ends_with_txt(file_path)) {
        std::cout << "No .txt" << std::endl;
        return false;
    }
    try {
        manager.open_file(dir_path + file_path, board, white, black);
    }
    catch (std::exception &e) {
        std::cout << "Couldn't open file!" << std::endl;
        return false;
    }
    white.fill_pieces(board);
    black.fill_pieces(board);
    app.game(board, white, black, false);
    return true;
}

void Menu::run() {
    std::cout << "Chess game\nChoose how you want to play.\n" << std::endl;
    print_menu();
    char in;
    std::cin >> in;
    int input_counter = 0;
    if (std::cin.eof()) {
        throw std::runtime_error("End of input");
    }
    std::cout << std::endl;
    while (true) {
        if (in == 'e') {
            std::cout << "End of program!" << std::endl;
            break;
        }
        // 10 wrong inputs = end program
        if (input_counter == 10) {
            std::cout << "You have entered 10 times in a row wrong input!" << std::endl;
            break;
        }
        input_counter++;
        switch (tolower(in)) {
            case 't': // Tutorial
                std::cout << "Tutorial" << std::endl;
                Tutorial tutorial;
                tutorial.run_tutorial();
                print_menu();
                break;
            case 'h': // Game vs human
                start_game();
                print_menu();
                break;
            case 'a': // Game vs AI
                if (!start_AI_game()) {
                    continue;
                }
                print_menu();
                break;
            case 'l': // Load game
                if (!load_game()) {
                    continue;
                }
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
}