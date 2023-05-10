#include <exception>
#include "application.h"

int main() {
    Application app;

    try {
        app.run();
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 1;
}