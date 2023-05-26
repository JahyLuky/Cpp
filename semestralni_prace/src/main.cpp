#include <exception>

#include "menu.h"

// TODO: doxygen correct

int main() {
    Menu menu;

    try {
        menu.run();
    }
    catch (std::exception &e) {
        std::cout << "main catch" << std::endl;
        std::cout << e.what() << std::endl;
    }

    return 0;
}