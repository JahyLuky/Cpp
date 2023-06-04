#include "file_manager.h"

void File_manager::place_piece_on_board(std::vector<Square> &pieces, char piece, const Position &pos) {
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

void File_manager::fill_pieces(Board &board, std::string &fen) {
    std::vector<Square> pieces;
    Position pos(0, 0);

    int end = 0;
    while (fen[end] != '\0') {
        if (fen[end] != '/') {
            if (std::isdigit(fen[end])) {
                int number = fen[end] - '0';
                for (int i = 0; i < number; ++i) {
                    place_piece_on_board(pieces, fen[end], pos);
                    // Moving piece to right
                    pos.col_++;
                }
            } else {
                place_piece_on_board(pieces, fen[end], pos);
                // Moving piece to right
                pos.col_++;
            }
        } else {
            board.squares_.emplace_back(std::move(pieces));

            // Moving piece down
            pos.row_++;
            pos.col_ = 0;
            pieces.clear();
        }
        end++;
    }

    board.squares_.emplace_back(std::move(pieces));
}

void File_manager::print_FEN(std::vector<std::string> &fields) {
    // Example output
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Piece Placement: " << fields[0] << std::endl;
    std::cout << "Active Color: " << fields[1] << std::endl;
    std::cout << "Castling Availability: " << fields[2] << std::endl;
    std::cout << "En Passant Target: " << fields[3] << std::endl;
    std::cout << "Halfmove Clock: " << fields[4] << std::endl;
    std::cout << "Fullmove Number: " << fields[5] << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}

void File_manager::readFEN(const std::string &fen, Board &board, Player &white, Player &black) {
    // Split the FEN notation into fields
    std::vector<std::string> fields;
    size_t start_pos = 0;
    size_t end_pos = fen.find(' ');

    // Extract each field from the FEN notation
    while (end_pos != std::string::npos) {
        // Extract the substring corresponding to the field
        fields.emplace_back(fen.substr(start_pos, end_pos - start_pos));
        start_pos = end_pos + 1;
        end_pos = fen.find(' ', start_pos);
    }

    fields.emplace_back(fen.substr(start_pos));

    // Piece placement data
    std::string piece_placement = fields[0];
    fill_pieces(board, piece_placement);

    // Active color
    std::string active_color = fields[1];
    if (active_color == "w") {
        board.white_playes = true;
    } else {
        board.white_playes = false;
    }

    // Castling availability
    std::string can_castle = fields[2];
    int w_row = white.king_.row_;
    int w_col = white.king_.col_;
    int b_row = black.king_.row_;
    int b_col = black.king_.col_;
    if (can_castle == "-") {
        board.squares_[w_row][w_col].piece_->first_move_ = true;
        board.squares_[b_row][b_col].piece_->first_move_ = true;
    } else {
        for (char c: can_castle) {
            if (c == 'K') {
                board.squares_[7][7].piece_->first_move_ = false;
            } else if (c == 'Q') {
                board.squares_[7][0].piece_->first_move_ = false;
            } else if (c == 'k') {
                board.squares_[0][7].piece_->first_move_ = false;
            } else if (c == 'q') {
                board.squares_[0][0].piece_->first_move_ = false;
            }
        }

    }

    // En passant target square
    std::string can_en_passant = fields[3];
    if (can_en_passant == "-") {
        if (active_color == "w") {
            white.en_passant_ = false;
        } else {
            black.en_passant_ = false;
        }
    } else {
        if (active_color == "w") {
            white.en_passant_ = true;
        } else {
            black.en_passant_ = true;
        }
    }

    // Halfmove clock
    std::string halfmove = fields[4];
    // TODO: no number edge case
    board.halfmoves_ = std::stoi(halfmove);

    // Fullmove number
    // TODO: do something about move count
    std::string fullmove = fields[5];

    board.print_color_board();

    print_FEN(fields);

}

bool File_manager::open_file(const std::string &file_path, Board &board, Player &white, Player &black) {
    std::ifstream ifs(file_path);

    if (!ifs.is_open()) {
        std::cout << "Failed to open the file." << std::endl;
        return false;
    }

    std::string fen;
    std::getline(ifs, fen);

    // Resets the board
    board.squares_.clear();

    readFEN(fen, board, white, black);

    ifs.close();

    return true;
}