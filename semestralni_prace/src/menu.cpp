#include "menu.h"

void Menu::run() const {
    std::cout << "Chess game\nChoose how you want to play.\n" << std::endl;

    std::cout << "------MENU------\nChoose your type of game:\n";
    std::cout << "(t):\tTutorial\n(h):\tPlayer vs Player\n(a):\tPlayer vs AI" << std::endl;
    char in;
    std::cin >> in;

    int input_counter = 0;

    // Check "ctrl+d"
    if (std::cin.eof()) {
        std::cout << "Input failed!" << std::endl;
        return;
    }
    std::cout << std::endl;

    bool game_on = true;
    while (game_on) {
        // 25 wrong inputs = end program
        if (input_counter == 25) {
            break;
        }
        input_counter++;
        switch (tolower(in)) {
            case 't': // tutorial
                std::cout << "Tutorial" << std::endl;
                game_on = false;
                break;
            case 'h': // game vs human
                std::cout << "Player vs Player" << std::endl;
                Application app;
                app.game();
                game_on = false;
                break;
            case 'a': // game vs AI
                std::cout << "Player vs AI" << std::endl;
                std::cout << "Choose AI: " << std::endl;
                game_on = false;
                break;
            default:
                std::cout << "Wrong input!" << std::endl;
                std::cin >> in;
                if (std::cin.eof()) {
                    std::cout << "Input failed!" << std::endl;
                    game_on = false;
                }
        }
    }
    std::cout << std::endl;
}

