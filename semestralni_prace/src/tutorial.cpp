#include "tutorial.h"

void Tutorial::run() {
    std::string file_path = "../semestralni_prace/examples/start_board.txt";

    File_manager manager;
    Board board;
    HumanPlayer white('W');
    HumanPlayer black('B');
    manager.open_file(file_path, board, white, black);

}
