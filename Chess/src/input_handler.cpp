#include "input_handler.h"

int Input_handler::coordinates_to_int(char c) {
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

char Input_handler::int_to_coordinates(int c) {
    switch (tolower(c)) {
        case 1:
            return 'a';
        case 2:
            return 'b';
        case 3:
            return 'c';
        case 4:
            return 'd';
        case 5:
            return 'e';
        case 6:
            return 'f';
        case 7:
            return 'g';
        case 8:
            return 'g';
        default:
            return 'x';
    }
}

int Input_handler::check_col(char c) {
    int num = c - '0';
    if (num < 1 || num > 8) {
        return -1;
    }
    return num;
}

bool Input_handler::extract_input(const std::string &start, Position &old_pos) {
    int col = coordinates_to_int(start[0]);
    int row = check_col(start[1]);

    if (row == -1 || col == -1
        || start.length() > 2) {
        return false;
    }
    old_pos.row_ = 8 - row;
    old_pos.col_ = col;
    return true;
}