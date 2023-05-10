#include "application.h"

int coordinates_to_int(char c) {
    switch (tolower(c)) {
        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
        default:
            return -1;
    }
}

int check_col(char c) {
    int num = c - '0';
    if (num < 1 || num > 8) {
        return -1;
    }
    return num;
}

bool extract_input(const std::string &start, const std::string &end, Position &old_pos, Position &new_pos) {
    int col = coordinates_to_int(start[0]);
    int row = check_col(start[1]);

    int col2 = coordinates_to_int(end[0]);
    int row2 = check_col(end[1]);

    if (row == -1 || col == -1 || row2 == -1 || col2 == -1
        || start.length() > 2 || end.length() > 2) {
        std::cout << "Coordinates out of range!" << std::endl;
        return false;
    }

    old_pos.row_ = 8 - row;
    old_pos.col_ = col;

    new_pos.row_ = 8 - row2;
    new_pos.col_ = col2;

    return true;
}

bool Application::run() const {
    Menu menu;

    switch (menu.print_menu()) {
        case 't':
            if (!tutorial()) {
                return false;
            }
        case 'a':
            break;
        case 'h':
            if (!game_vs_human()) {
                return false;
            }
        default:
            return false;
    }

    return true;
}

bool Application::tutorial() const {
    std::cout << "Pravidla:" << std::endl;
    if (!game_vs_human()) {
        return false;
    }
    return true;
}


bool Application::game_vs_human() const {
    Board board;

    board.init_board();

    /*
    std::cout << "\nNo color and format board\n" << std::endl;
    board.print_basic_board();
    std::cout << "\nColor and format board\n" << std::endl;
    board.print_color_board();
    */
    std::cout << "\nGame starts!\n" << std::endl;

    // print classical board
    board.print_color_board();

    std::string start, end;
    Position old_pos{}, new_pos{};
    HumanPlayer A{}, B{};
    bool white_plays = true;
    while (!board.game_over) {
        std::cout << "What is your move?" << std::endl;
        std::cin >> start;
        std::cin >> end;

        if (std::cin.eof()) {
            std::cout << "Game over!" << std::endl;
            break;
        }

        if (!extract_input(start, end, old_pos, new_pos)) {
            return false;
        }

        // change turns
        if (white_plays) {
            A.make_move(board, old_pos, new_pos);
            white_plays = false;
        } else {
            B.make_move(board, old_pos, new_pos);
            white_plays = true;
        }
        // print updated board
        board.print_color_board();
    }
    return true;
}