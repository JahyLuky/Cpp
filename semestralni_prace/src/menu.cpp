#include "menu.h"

bool Menu::print_menu() {
    std::cout << "\nChoose how you want to play.\n" << std::endl;

    std::cout << "Menu:\nChoose your game:\n(t):\tTutorial\n(h):\tGame vs human\n(a):\tGame vs AI" << std::endl;
    char in;
    std::cin >> in;

    // Check "ctrl+d"
    if (std::cin.eof()) {
        std::cout << "Input failed!" << std::endl;
        return false;
    }
    std::cout << std::endl;

    switch (tolower(in)) {
        case 't': // tutorial
            std::cout << "Tutorial" << std::endl;
            break;
        case 'h': // game vs human
            std::cout << "Game vs human" << std::endl;
            break;
        case 'a': // game vs AI
            std::cout << "Game vs AI" << std::endl;
            std::cout << "Choose your AI: " << std::endl;
            break;
        default:
            std::cout << "Wrong input!" << std::endl;
            return false;
    }
    std::cout << std::endl;

    // Choosing board format
    /*
    std::cout << "Choose your board format:\n(c):\tColored\n(b:)\tBasic" << std::endl;
    std::cin >> in;
    // Check "ctrl+d"
    if (std::cin.eof()) {
        std::cout << "Input failed!" << std::endl;
        return false;
    }

    if (in == 'c'){
        is_colored_board_ = true;
    } else if (in == 'b') {
        is_colored_board_ = false;
    } else {
        std::cout << "Wrong format input!" << std::endl;
        return false;
    }
    */
    return true;
}