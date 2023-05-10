#include "menu.h"

char Menu::print_menu() const {
    std::cout << "Chess game\nChoose how you want to play.\n" << std::endl;

    std::cout << "------MENU------\nChoose your type of game:\n";
    std::cout << "(t):\tTutorial\n(h):\tGame vs human\n(a):\tGame vs AI" << std::endl;
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
            return 'f';
    }
    std::cout << std::endl;
    return in;
}