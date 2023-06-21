#include "human_player.h"
#include "input_handler.h"

HumanPlayer::HumanPlayer(char white_plays)
        : Player(white_plays) {}

bool HumanPlayer::get_promotion(char &piece) {
    std::string in;
    std::cout << "To what piece u want to promote? (q, b, n, r)" << std::endl;
    // Get positions
    std::cin >> in;
    if (in.length() > 1) {
        std::cout << "Enter just 1 word!" << std::endl;
        return false;
    }
    Input_handler input_handler;
    if (!input_handler.coordinates_to_int(in[0])) {
        std::cout << "Wrong piece!" << std::endl;
        return false;
    }
    if (tolower(in[0]) == 'k' || tolower(in[0]) == 'p') {
        std::cout << "Wrong piece!" << std::endl;
        return false;
    }
    piece = in[0];
    return true;
}

bool HumanPlayer::get_move(Board &board) {
    std::string start, end;

    // Get positions
    std::cin >> start;

    if (start == "save") {
        ask_draw_ = save;
        return true;
    }

    if (start == "resign") {
        ask_draw_ = resign;
        return true;
    }

    if (start == "draw") {
        ask_draw_ = draw_asked;
        return true;
    }

    if (start == "yes") {
        ask_draw_ = draw;
        return true;
    }

    if (start == "no") {
        ask_draw_ = no_draw;
        return true;
    }

    Input_handler input_handler;

    if (!input_handler.extract_input(start, start_)) {
        return false;
    }

    std::cin >> end;

    if (std::cin.eof()) {
        throw std::runtime_error("End of player's input");
    }

    if (!input_handler.extract_input(end, end_)) {
        return false;
    }

    return true;
}

