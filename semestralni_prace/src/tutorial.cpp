#include "tutorial.h"

void Tutorial::place_piece_on_board(std::vector<Square> &pieces, char piece, const Position &pos) {
    char color = 'W';
    if (islower(piece)) {
        color = 'B';
    }

    int lower_piece = tolower(piece);
    if (lower_piece == 'r') {
        Square tmp(pos, std::make_unique<Rook>(piece, color, pos));
        pieces.emplace_back(std::move(tmp));
    } else if (lower_piece == 'n') {
        Square tmp(pos, std::make_unique<Knight>(piece, color, pos));
        pieces.emplace_back(std::move(tmp));
    } else if (lower_piece == 'b') {
        Square tmp(pos, std::make_unique<Bishop>(piece, color, pos));
        pieces.emplace_back(std::move(tmp));
    } else if (lower_piece == 'q') {
        Square tmp(pos, std::make_unique<Queen>(piece, color, pos));
        pieces.emplace_back(std::move(tmp));
    } else if (lower_piece == 'k') {
        Square tmp(pos, std::make_unique<King>(piece, color, pos));
        pieces.emplace_back(std::move(tmp));
    } else if (lower_piece == 'p') {
        Square tmp(pos, std::make_unique<Pawn>(piece, color, pos));
        pieces.emplace_back(std::move(tmp));
    } else {
        pieces.emplace_back(pos, nullptr);
    }
}

bool Tutorial::read_file(std::ifstream &ifs) {
    Board board;
    std::vector<Square> pieces;
    Position pos(0, 0);
    int FEN_cnt = 0;
    for (char FEN_piece; ifs.get(FEN_piece);) {
        if (ifs.fail()) {
            std::cout << "Fail bit!" << std::endl;
            return false;
        }

        if (FEN_piece != '/') {
            if (FEN_piece == '8') {
                for (int i = 0; i < 8; ++i) {
                    place_piece_on_board(pieces, FEN_piece, pos);
                    // Moving piece to right
                    pos.col_++;
                }
            } else {
                place_piece_on_board(pieces, FEN_piece, pos);
                // Moving piece to right
                pos.col_++;
            }
        } else {
            board.squares_.emplace_back(std::move(pieces));

            // Moving piece down
            pos.row_++;
            pos.col_ = 0;
            FEN_cnt++;

            pieces.clear();
        }
    }

    board.print_color_board();

    // Check if the end of file was reached or if some other error occurred
    if (!ifs.eof()) {
        std::cout << "Error reading file." << std::endl;
        return false;
    }

    return true;
}


void Tutorial::run() {
    std::string file_path = "examples/start_board.txt";
    std::ifstream ifs(file_path);

    if (ifs.is_open()) {
        std::cout << "File opened successfully!" << std::endl;

        read_file(ifs);

        ifs.close();
    } else {
        std::cout << "Failed to open the file." << std::endl;
    }


}
