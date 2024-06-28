#include "file_manager.h"

#include <filesystem>
#include <stdexcept>

#include "input_handler.h"
#include "fen.h"

bool File_manager::ends_with_txt(const std::string &str) {
    std::string extension = ".txt";
    if (str.length() >= extension.length()) {
        std::string endOfString = str.substr(str.length() - extension.length());
        return endOfString.compare(extension) == 0;
    }
    return false;
}

void File_manager::list_directory(std::set<std::string> &dir_file_names, const std::string &dir_path) {
    std::cout << "\nCurrent files:" << std::endl;
    for (const auto &file: std::filesystem::directory_iterator(dir_path)) {
        dir_file_names.insert(file.path().filename().string());
        std::cout << file.path().filename() << std::endl;
    }
    std::cout << std::endl;
}

void File_manager::save_file(Board &board, Player &white, Player &black) {
    std::string dir_path = "examples/";
    std::set<std::string> dir_file_names;
    list_directory(dir_file_names, dir_path);
    std::string file_name;
    int wrong_input = 0;
    while (true) {
        if (wrong_input == 10) {
            throw std::runtime_error("You entered invalid name 10 times in a row!");
        }
        std::cout << "Enter file name with (.txt)" << std::endl;
        std::cin >> file_name;
        if (std::cin.eof()) {
            throw std::runtime_error("End of save input");
        }
        wrong_input++;
        auto itr = dir_file_names.find(file_name);
        if (itr != dir_file_names.end()) {
            std::cout << "This file already exists. Try again." << std::endl;
            continue;
        } else {
            if (!ends_with_txt(file_name)) {
                std::cout << "Invalid input (no .txt)!" << std::endl;
                continue;
            }
            break;
        }
    }
    std::ofstream output_file_stream(dir_path + file_name);
    if (!output_file_stream.is_open()) {
        throw std::runtime_error("Failed to open the file.");
    }
    FEN fen;
    output_file_stream << fen.get_FEN(board, white, black) << std::endl;
    output_file_stream.close();
}

void File_manager::open_file(const std::string &file_path, Board &board, Player &white, Player &black) {
    std::ifstream ifs(file_path);

    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open the file.");
    }

    std::string fen_notation;
    std::getline(ifs, fen_notation);

    // Resets the board
    board.squares_.clear();

    FEN fen;

    fen.read_FEN(fen_notation, board, white, black);

    ifs.close();
}