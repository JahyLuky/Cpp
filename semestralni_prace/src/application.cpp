#include "application.h"

void print_color(bool white_plays) {
    if (white_plays) {
        std::cout << "\nWHITE plays!" << std::endl;
    } else {
        std::cout << "\nBLACK plays!" << std::endl;
    }
}

void move_piece(Board &board, Square &start, Square &end) {
    // Move piece to new position
    end.piece_ = std::unique_ptr<Piece>(start.piece_->clone());
    // Change coor to new position
    end.piece_->set_position(Position(end.pos_.row_, end.pos_.col_));
    // Empty starting square
    start.piece_ = nullptr;
}

void handle_castling(Board &board, Player &player, Square &start, Square &end) {
    // Move king
    move_piece(board, start, end);

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

    move_piece(board, rook_start, rook_end);

    player.castling_ = false;
}

void Application::make_move(Board &board, Player &player) const {
    // Starting position of piece
    Square &start = board.squares_[player.start_.row_][player.start_.col_];

    // Final position of piece
    Square &end = board.squares_[player.end_.row_][player.end_.col_];

    bool promote = false;

    // No double move
    if (tolower(start.piece_->get_piece()) == 'p') {
        start.piece_->first_move_ = false;
        if ((player.end_.row_ % 7) == 0) {
            promote = true;
        }
    }

    // TODO: piece in way or check
    // Castling
    if (player.castling_) {
        handle_castling(board, player, start, end);
        return;
    }

    // No castle
    if (tolower(start.piece_->get_piece()) == 'k') {
        start.piece_->first_move_ = false;
        player.king_ = end.pos_;
    }
    if (tolower(start.piece_->get_piece()) == 'r') {
        start.piece_->first_move_ = false;
    }

    if (!promote) {
        // Taken piece
        if (end.piece_ != nullptr) {
            player.captures_.push_back(end.piece_->get_piece());
        }
        move_piece(board, start, end);
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
        move_piece(board, start, end);
    }
}

bool Application::play_move(Board &board, Player &player) const {
    Rules rules;

    // Ask player for move
    if (!player.get_move()) {
        return false;
    }
    // Check if player give valid move
    if (!rules.validate_move(board, player)) {
        return false;
    }

    // TODO: change to undo_move()
    Board is_check = board;

    bool castle = false;
    if (player.castling_) {
        castle = true;
    }

    Position king(player.king_);

    make_move(is_check, player);
    Position pos(player.king_.row_, player.king_.col_);

    player.castling_ = castle;
    player.captures_.clear();
    player.king_ = king;

    // Check if player's king is in check
    if (!rules.checked(is_check, pos, player.color_)) {
        make_move(board, player);

        // Save last move (for en passant)
        board.start_ = player.start_;
        board.end_ = player.end_;
    } else {
        std::cout << "White king is in check" << std::endl;
        return false;
    }

    return true;
}

bool Application::game() const {
    std::cout << "\nGame starts!\n" << std::endl;
    std::cout << "\nWHITE plays!" << std::endl;

    Board board;
    board.init_board();
    board.print_color_board();

    HumanPlayer A('W'), B('B');
    bool white_plays = true;

    // TODO: find all possible TIEs
    while (!board.game_over) {
        std::cout << "What is your move?" << std::endl;

        if (std::cin.eof()) {
            std::cout << "End of input or file!" << std::endl;
            break;
        }

        // TODO: based on game_type, change get old and new pos from input or AI

        // Change turns
        if (white_plays) {
            if (play_move(board, A)) {
                white_plays = false;
            } else {
                continue;
            }
        } else {
            if (play_move(board, B)) {
                white_plays = true;
            } else {
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
        print_color(white_plays);
    }
    return true;
}