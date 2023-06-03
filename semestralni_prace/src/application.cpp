#include "application.h"

void Application::print_color(bool white_plays) {
    if (white_plays) {
        std::cout << "\nWHITE plays!" << std::endl;
    } else {
        std::cout << "\nBLACK plays!" << std::endl;
    }
}

void Application::move_piece(Square &start, Square &end) {
    // Move piece to new position
    end.piece_ = std::unique_ptr<Piece>(start.piece_->clone());

    // Change coor to new position
    end.piece_->set_position(Position(end.pos_.row_, end.pos_.col_));

    // Empty starting square
    start.piece_ = nullptr;
}

void Application::handle_castling(Board &board, Player &player, Square &start, Square &end) {
    // Move king
    move_piece(start, end);

    // 3 -> left
    int side = 3;
    // first col
    int rook_col = 0;
    if (end.pos_.col_ == 6) {
        // -2 -> right
        side = -2;
        // last col
        rook_col = 7;
    }

    Square &rook_start = board.squares_[player.end_.row_][rook_col];
    Square &rook_end = board.squares_[player.end_.row_][rook_col + side];

    move_piece(rook_start, rook_end);

    player.castling_ = false;
}

void Application::make_move(Board &board, Player &player) {
    // Starting position of piece
    Square &start = board.squares_[player.start_.row_][player.start_.col_];

    // Final position of piece
    Square &end = board.squares_[player.end_.row_][player.end_.col_];

    bool promote = false;

    // No double move
    if (tolower(start.piece_->get_piece()) == 'p') {
        start.piece_->first_move_ = true;
        if ((player.end_.row_ % 7) == 0) {
            promote = true;
        }
    }

    // TODO: piece in a way or check
    // Castling
    if (player.castling_) {
        handle_castling(board, player, start, end);
        return;
    }

    // King's first move
    if (tolower(start.piece_->get_piece()) == 'k') {
        start.piece_->first_move_ = true;
        player.king_ = end.pos_;
    }

    // Rook's first move
    if (tolower(start.piece_->get_piece()) == 'r') {
        start.piece_->first_move_ = true;
    }

    if (!promote) {
        // Taken piece
        if (end.piece_ != nullptr) {
            player.captures_.push_back(end.piece_->get_piece());
        }
        move_piece(start, end);
    } else {
        // TODO: make proper promotion
        char q;
        if (player.color_ == 'W') {
            q = 'q';
        } else {
            q = 'Q';
        }
        // Taken piece
        if (end.piece_ != nullptr) {
            player.captures_.push_back(end.piece_->get_piece());
        }
        start.piece_ = std::make_unique<Queen>(q, player.color_, Position(player.end_.row_, player.end_.col_));
        move_piece(start, end);
    }
}

void Application::undo_move(Board &board, Player &player) {
    Position tmp = player.start_;
    player.start_ = player.end_;
    player.end_ = tmp;
    make_move(board, player);
}

bool Application::play_move(Board &board, Player &player) {
    Rules rules;

    // Ask player for move
    if (!player.get_move()) {
        return false;
    }

    // Validate player's move
    if (!rules.validate_move(board, player)) {
        return false;
    }

    size_t captures = player.captures_.size();
    Position king(player.king_);
    bool castle = player.castling_;
    bool en_passant = player.en_passant_;

    make_move(board, player);

    Position pos(player.king_.row_, player.king_.col_);

    // Check if player's king is in check
    if (!rules.checked(board, pos, player.color_)) {
        // Save last move (for en passant)
        board.start_ = board.squares_[player.start_.row_][player.start_.col_];
        board.end_ = board.squares_[player.end_.row_][player.end_.col_];

        if (player.en_passant_) {
            if (player.end_.row_ == 2) {
                // Empty square
                player.captures_.emplace_back('P');
                board.squares_[player.end_.row_ + 1][player.end_.col_].piece_ = nullptr;
            }
            if (player.end_.row_ == 5) {
                // Empty square
                player.captures_.emplace_back('p');
                board.squares_[player.end_.row_ - 1][player.end_.col_].piece_ = nullptr;
            }
            player.en_passant_ = false;
        }
    } else {
        player.castling_ = castle;
        player.en_passant_ = en_passant;
        // TODO: check if capture was made, delete it if undo needed
        if (captures != player.captures_.size()) {
            player.captures_.pop_back();
        }
        player.king_ = king;

        undo_move(board, player);
        std::cout << "Your king is in check" << std::endl;
        return false;
    }

    return true;
}

bool Application::game() {
    std::cout << "\nGame starts!\n" << std::endl;
    std::cout << "\nWHITE plays!" << std::endl;

    Board board;
    board.init_board();
    board.print_color_board();

    HumanPlayer A('W'), B('B');

    // TODO: find all possible TIEs
    while (!board.game_over) {
        std::cout << "What is your move?" << std::endl;

        if (std::cin.eof()) {
            std::cout << "End of input or file!" << std::endl;
            break;
        }

        // TODO: based on game_type, change get old and new pos from input or AI

        // Change turns
        if (board.white_playes) {
            if (play_move(board, A)) {
                // Piece moved, switch players
                board.white_playes = false;
            } else {
                // Incorrect move, try again
                continue;
            }
        } else {
            if (play_move(board, B)) {
                // Piece moved, switch players
                board.white_playes = true;
            } else {
                // Incorrect move, try again
                continue;
            }
        }

        // TODO: captures resets, need to fix
        A.print_captures();
        std::cout << std::endl;
        B.print_captures();
        std::cout << std::endl;

        // Print updated board
        board.print_color_board();
        // Print who plays next
        print_color(board.white_playes);
    }
    return true;
}