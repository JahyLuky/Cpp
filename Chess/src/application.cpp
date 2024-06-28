#include "application.h"
#include <stdexcept>
#include "file_manager.h"
#include "validate.h"

void Application::print_color(const bool white_plays) const {
    if (white_plays) {
        std::cout << "\nWHITE plays!" << std::endl;
    } else {
        std::cout << "\nBLACK plays!" << std::endl;
    }
    std::cout << "What is your move?" << std::endl;
}

bool Application::play_move(Board &board, Player &player) {
    Rules rules;
    Move move;
    Validate validate;

    // Backup for undo_move
    Position king(player.king_);
    bool castle = player.castling_;
    bool en_passant = player.en_passant_;
    std::vector<Position> pieces_pos_save = player.pieces_pos_;

    // King isn't in check
    if (!rules.checked(board, king, player.color_)) {
        // Print who plays next
        print_color(board.white_playes);

        // Ask player for move
        if (!player.get_move(board)) {
            return false;
        }
        // Check if player offers draw
        if (player.ask_draw_ != 0) {
            return true;
        }
        // Validate player's move
        if (!validate.validate_move(board, player, true)) {
            return false;
        }
        Square & king_piece = board.squares_[player.start_.row_][player.start_.col_];
        if (tolower(king_piece.piece_->get_piece()) == 'k') {
            player.king_ = player.end_;
        }
        move.make_move(board, player);
        if (rules.checked(board, player.king_, player.color_)) {
            player.castling_ = castle;
            player.en_passant_ = en_passant;
            player.king_ = king;
            player.pieces_pos_ = pieces_pos_save;
            move.undo_move(board, player);
            std::cout << "Your king would be in check, try again." << std::endl;
            return false;
        }
        // Save last move
        board.start_ = board.squares_[player.start_.row_][player.start_.col_];
        board.end_ = board.squares_[player.end_.row_][player.end_.col_];
        // En passant was played
        if (player.en_passant_) {
            if (player.end_.row_ == 2) {
                // Empty square
                board.squares_[player.end_.row_ + 1][player.end_.col_].piece_ = nullptr;
            }
            if (player.end_.row_ == 5) {
                // Empty square
                board.squares_[player.end_.row_ - 1][player.end_.col_].piece_ = nullptr;
            }
            player.en_passant_ = false;
        }
    } else {
        // King in checkmate
        if (rules.checkmate(board, player)) {
            std::cout << "CHECKMATE!" << std::endl;
            if (player.color_ == 'W') {
                board.game_over = black_wins;
            } else {
                board.game_over = white_wins;
            }
            return false;
        }
        // King in check
        // Print who plays next
        print_color(board.white_playes);
        // Ask player for move
        if (!player.get_move(board)) {
            return false;
        }
        // Check if player offers draw
        if (player.ask_draw_ != 0) {
            return true;
        }
        // Validate player's move
        if (!validate.validate_move(board, player, true)) {
            return false;
        }
        Square & king_piece = board.squares_[player.start_.row_][player.start_.col_];
        if (tolower(king_piece.piece_->get_piece()) == 'k') {
            player.king_ = player.end_;
        }
        move.make_move(board, player);
        if (rules.checked(board, player.king_, player.color_)) {
            player.castling_ = castle;
            player.en_passant_ = en_passant;
            player.king_ = king;
            player.pieces_pos_ = pieces_pos_save;
            move.undo_move(board, player);
            std::cout << "Your king is still in check, try again." << std::endl;
            return false;
        }
        return true;
    }
    return true;
}

void Application::print_results(const Board &board) const {
    switch (board.game_over) {
        case game_not_finished:
            std::cout << "Game wasn't finished!" << std::endl;
            break;
        case white_wins:
            std::cout << "White wins!" << std::endl;
            break;
        case black_wins:
            std::cout << "Black wins!" << std::endl;
            break;
        case tie:
            std::cout << "Tie!" << std::endl;
            break;
        case pat:
            std::cout << "Pat!" << std::endl;
            break;
        default:
            break;
    }
}

bool Application::change_turns(Board &board, Player &white, Player &black, int &wrong_move_cnt, bool is_tutorial) {
    if (board.white_playes) {
        if (black.ask_draw_ == save) {
            board.game_over = save_game;
            return false;
        }
        if (black.ask_draw_ == resign) {
            board.game_over = white_wins;
            return false;
        }
        if (black.ask_draw_ == draw_asked) {
            std::cout << "Black offers a draw." << std::endl;
            std::cout << "Type (yes) for accepting draw." << std::endl;
            std::cout << "Type (no) for denying draw." << std::endl;
            play_move(board, white);
            if (board.game_over == black_wins) {
                return true;
            }
            if (white.ask_draw_ == draw) {
                board.game_over = tie;
            } else {
                white.ask_draw_ = no_draw_asked;
                black.ask_draw_ = no_draw_asked;
                board.white_playes = false;
                return true;
            }

            return false;
        }
        if (play_move(board, white)) {
            // Switch turns
            board.white_playes = false;
            wrong_move_cnt = 0;
        } else {
            // Incorrect move, try again
            wrong_move_cnt++;
            return false;
        }
    } else {
        if (white.ask_draw_ == save) {
            board.game_over = save_game;
            return false;
        }
        if (white.ask_draw_ == resign) {
            board.game_over = black_wins;
            return false;
        }
        if (white.ask_draw_ == draw_asked) {
            std::cout << "White offers a draw." << std::endl;
            std::cout << "Type (yes) for accepting draw." << std::endl;
            std::cout << "Type (no) for denying draw." << std::endl;
            play_move(board, black);
            if (board.game_over == white_wins) {
                return true;
            }
            if (black.ask_draw_ == draw) {
                board.game_over = tie;
            } else {
                if (is_tutorial) {
                    board.game_over = tutorial;
                    return false;
                }
                white.ask_draw_ = no_draw_asked;
                black.ask_draw_ = no_draw_asked;
                board.white_playes = true;
                return true;
            }
            return false;
        }
        if (is_tutorial) {
            board.game_over = tutorial;
            return false;
        }
        if (play_move(board, black)) {
            // Switch turns
            board.white_playes = true;
            wrong_move_cnt = 0;
        } else {
            // Incorrect move, try again
            wrong_move_cnt++;
            return false;
        }
    }
    return true;
}

void Application::game(Board &board, Player &white, Player &black, bool is_tutorial) {
    std::cout << "\nGame starts!\n" << std::endl;
    GameRules game_rules;
    board.print_board();
    int wrong_move_cnt = 0, move_cnt = 0;
    // Threefold repetition
    std::vector<Position> white_last_moves;
    std::vector<Position> black_last_moves;

    while (true) {
        // TODO: check tutorial
        if (is_tutorial && move_cnt == 2) {
            board.game_over = tutorial;
            break;
        }
        if (wrong_move_cnt == 10) {
            std::cout << "\n\nYou entered 10 times in a row wrong input, maybe try tutorial first.\n\n" << std::endl;
            break;
        }
        if (board.halfmoves_ >= 50) {
            board.game_over = tie;
            break;
        }
        if (board.game_over != game_not_finished) {
            break;
        }
        if (std::cin.eof()) {
            throw std::runtime_error("End of input");
        }
        if (game_rules.stalemate(board, white, black)) {
            board.game_over = pat;
            break;
        }
        // Change turns
        if (!change_turns(board, white, black, wrong_move_cnt, is_tutorial)) {
            continue;
        }
        game_rules.update_threefold(board, white, black, white_last_moves, black_last_moves);
        game_rules.half_moves(board, white, black);
        if (game_rules.dead_position(board, white, black)) {
            board.game_over = tie;
            break;
        }
        // Check Threefold repetition
        if (game_rules.threefold(white_last_moves, black_last_moves)) {
            board.game_over = tie;
            break;
        }
        if (board.taken_.piece_ != nullptr ) {
            if (board.white_playes) {
                white.delete_piece(board.taken_.pos_);
            } else {
                black.delete_piece(board.taken_.pos_);
            }
        }
        // Reset taken piece for undo_move -> move was legit
        board.taken_.piece_ = nullptr;
        board.fullmoves_++;
        move_cnt++;
        // Print updated board
        board.print_board();
    }
    if (board.game_over == save_game) {
        File_manager file_manager;
        file_manager.save_file(board, white, black);
    }
    print_results(board);
}