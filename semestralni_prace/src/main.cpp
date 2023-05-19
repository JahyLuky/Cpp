#include <exception>
#include "application.h"

// TODO: doxygen correct

int main() {
    Application app;

    try {
        app.run();
    }
    catch (std::exception &e) {
        std::cout << "main catch" << std::endl;
        std::cout << e.what() << std::endl;
    }

    return 1;
}