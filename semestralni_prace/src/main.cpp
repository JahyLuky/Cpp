#include "board.h"
#include "move.h"
#include "player.h"
#include "piece.h"
#include "menu.h"

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

bool extract_input(const std::string &start,const std::string &end, Position &old_pos, Position &new_pos) {
    int col = coordinates_to_int(start[0]);
    int row = check_col(start[1]);

    int col2 = coordinates_to_int(end[0]);
    int row2 = check_col(end[1]);

    if (row == -1 || col == -1 || row2 == -1 || col2 == -1) {
        std::cout << "Coordinates out of range!" << std::endl;
        return false;
    }

    old_pos.row_ = 8 - row;
    old_pos.col_ = col;

    new_pos.row_ = 8 - row2;
    new_pos.col_ = col2;

    return true;
}

int main() {
    std::cout << "\nWelcome to chess world!\n" << std::endl;

    if (!Menu::print_menu()) {
        return 0;
    }

    Board board;
    board = Board::init_board();
    Board::print_board(board);

    std::string start;
    std::string end;
    Position old_pos{};
    Position new_pos{};
    while (!board.game_over) {
        std::cout << "What is your move?" << std::endl;
        std::cin >> start;
        std::cin >> end;

        if (std::cin.eof()) {
            std::cout << "Game over!" << std::endl;
            return 0;
        }

        if (!extract_input(start, end, old_pos, new_pos)) {
            return 0;
        }

        Player::make_move(board, old_pos, new_pos);
        //Player::make_move(board, Position(6,4), Position(4,4));
        Board::print_board(board);

    }

    return 1;
}