#include "fen.h"

#include <stdexcept>
#include "input_handler.h"

std::string FEN::get_piece_notation(const Piece *piece) {
    if (piece == nullptr) {
        return "";
    }
    char pieceChar = piece->get_piece();
    if (piece->get_color() == 'B') {
        pieceChar = std::tolower(pieceChar);
    }
    return std::string(1, pieceChar);
}

std::string FEN::save_pieces(const Board &board) {
    std::string piecePlacement;
    for (int row = 0; row < 8; ++row) {
        int emptySquaresCount = 0;
        for (int col = 0; col < 8; ++col) {
            const Piece *piece = board.squares_[row][col].piece_.get();
            std::string pieceNotation = get_piece_notation(piece);
            if (pieceNotation.empty()) {
                emptySquaresCount++;
            } else {
                if (emptySquaresCount > 0) {
                    piecePlacement += std::to_string(emptySquaresCount);
                    emptySquaresCount = 0;
                }
                piecePlacement += pieceNotation;
            }
        }
        if (emptySquaresCount > 0) {
            piecePlacement += std::to_string(emptySquaresCount);
        }
        if (row < 7) {
            piecePlacement += '/';
        }
    }
    return piecePlacement;
}

std::string FEN::get_FEN(Board &board, const Player &white, const Player &black) {
    std::string fen;
    // Piece placement
    fen += save_pieces(board);
    // Active color
    fen += ' ';
    char color = 'w';
    if (board.white_playes) {
        color = 'b';
    }
    fen += color;
    // Castling availability
    fen += ' ';
    // Check if any rooks or kings have moved to determine castling availability
    if (board.squares_[7][7].piece_ != nullptr && !board.squares_[7][7].piece_->first_move_) {
        fen += 'K';
    }
    if (board.squares_[7][0].piece_ != nullptr && !board.squares_[7][0].piece_->first_move_) {
        fen += 'Q';
    }
    if (board.squares_[0][7].piece_ != nullptr && !board.squares_[0][7].piece_->first_move_) {
        fen += 'k';
    }
    if (board.squares_[0][0].piece_ != nullptr && !board.squares_[0][0].piece_->first_move_) {
        fen += 'q';
    }
    if (fen.back() == ' ') {
        fen += '-';
    }
    Input_handler input_handler;
    // En passant target
    fen += ' ';
    if (color == 'w') {
        Square &square = board.squares_[black.end_.row_][black.end_.col_];
        if (square.piece_ != nullptr
            && tolower(square.piece_->get_piece()) == 'p'
            && (black.end_.row_ - black.start_.row_) == 2) {
            char col = input_handler.int_to_coordinates(black.end_.col_ + 1);
            fen += col;
            fen += "6";
        } else {
            fen += '-';
        }
    } else {
        Square &square = board.squares_[white.end_.row_][white.end_.col_];
        if (square.piece_ != nullptr
            && tolower(square.piece_->get_piece()) == 'p'
            && (white.start_.row_ - white.end_.row_) == 2) {
            char col = input_handler.int_to_coordinates(white.end_.col_ + 1);
            fen += col;
            fen += "3";
        } else {
            fen += '-';
        }
    }
    // Halfmove clock
    fen += ' ';
    fen += std::to_string(board.halfmoves_);
    // Fullmove number
    fen += ' ';
    fen += std::to_string(board.fullmoves_);
    return fen;
}

bool FEN::correct_FEN_pieces(const std::string &str) {
    int count = 0;
    int square_cnt = 0;
    for (char c: str) {
        if (c == '/') {
            count++;
            square_cnt = 0;
        } else if (c == 'r' || c == 'n' || c == 'b' || c == 'k' || c == 'q' || c == 'p'
                   || c == 'R' || c == 'N' || c == 'B' || c == 'K' || c == 'Q' || c == 'P') {
            square_cnt++;
        } else if (std::isdigit(c)) {
            square_cnt = c - '0';
        } else {
            return false;
        }
    }
    if (count != 7) {
        return false;
    }
    return true;
}

void FEN::place_piece_on_board(std::vector<Square> &pieces, char piece, const Position &pos, Player &white, Player &black) {
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
        // Set player king position
        if (color == 'W') {
            white.king_ = pos;
        } else {
            black.king_ = pos;
        }
    } else if (lower_piece == 'p') {
        Square tmp(pos, std::make_unique<Pawn>(piece, color, pos));
        pieces.emplace_back(std::move(tmp));
    } else {
        pieces.emplace_back(pos, nullptr);
    }
}

void FEN::fill_pieces(Board &board, std::string &fen, Player &white, Player &black) {
    std::vector<Square> pieces;
    Position pos(0, 0);
    int end = 0;
    while (fen[end] != '\0') {
        if (fen[end] != '/') {
            if (std::isdigit(fen[end])) {
                int number = fen[end] - '0';
                for (int i = 0; i < number; ++i) {
                    place_piece_on_board(pieces, fen[end], pos, white, black);
                    // Moving piece to right
                    pos.col_++;
                }
            } else {
                place_piece_on_board(pieces, fen[end], pos, white, black);
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

void FEN::read_FEN(const std::string &fen, Board &board, Player &white, Player &black) {
    Input_handler input_handler;
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
    if (!correct_FEN_pieces(piece_placement)) {
        throw std::runtime_error("Wrong pieces in FEN!");
    }
    fill_pieces(board, piece_placement, white, black);
    // Active color
    std::string active_color = fields[1];
    if (active_color == "w") {
        board.white_playes = true;
    } else {
        if (active_color == "b") {
            board.white_playes = false;
        } else {
            throw std::runtime_error("Wrong color in FEN!");
        }
    }
    // Castling availability
    std::string can_castle = fields[2];
    int w_row = white.king_.row_;
    int w_col = white.king_.col_;
    int b_row = black.king_.row_;
    int b_col = black.king_.col_;
    // No castle available
    if (can_castle == "-") {
        board.squares_[w_row][w_col].piece_->first_move_ = true;
        board.squares_[b_row][b_col].piece_->first_move_ = true;
    } else {
        for (char c: can_castle) {
            if (c == 'K') {
                // White rook moved, cant castle king-side
                if (board.squares_[7][7].piece_ == nullptr) {
                    throw std::runtime_error("Wrong castling rights!");
                }
                board.squares_[7][7].piece_->first_move_ = false;
            } else if (c == 'Q') {
                // White rook moved, cant castle queen-side
                if (board.squares_[7][0].piece_ == nullptr) {
                    throw std::runtime_error("Wrong castling rights!");
                }
                board.squares_[7][0].piece_->first_move_ = false;
            } else if (c == 'k') {
                // Black rook moved, cant castle king-side
                if (board.squares_[0][7].piece_ == nullptr) {
                    throw std::runtime_error("Wrong castling rights!");
                }
                board.squares_[0][7].piece_->first_move_ = false;
            } else if (c == 'q') {
                // Black rook moved, cant castle queen-side
                if (board.squares_[0][0].piece_ == nullptr) {
                    throw std::runtime_error("Wrong castling rights!");
                }
                board.squares_[0][0].piece_->first_move_ = false;
            } else {
                throw std::runtime_error("Wrong castling rights!");
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
        if (can_en_passant.length() == 2) {
            if (input_handler.coordinates_to_int(can_en_passant[0]) == -1
                || input_handler.check_col(can_en_passant[1]) == -1) {
                throw std::runtime_error("En passant wrong coordinates!");
            }
        } else {
            throw std::runtime_error("En passant wrong coordinates!");
        }
        if (active_color == "w") {
            white.en_passant_ = true;
        } else {
            black.en_passant_ = true;
        }
    }
    // Halfmove clock
    std::string halfmove_fen = fields[4];
    try {
        int halfmove = std::stoi(halfmove_fen);
        board.halfmoves_ = halfmove;
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error("Wrong halfmove number!");
    }
    // Fullmove clock
    std::string fullmove_fen = fields[5];
    try {
        int fullmove = std::stoi(fullmove_fen);
        board.fullmoves_ = fullmove;
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error("Wrong fullmove number!");
    }
}