#include "rules.h"
#include "validate.h"

const std::vector<Position> move_directions = {
        Position(1, 0),
        Position(0, 1),
        Position(-1, 0),
        Position(0, -1),
        Position(1, 1),
        Position(-1, -1),
        Position(-1, 1),
        Position(1, -1)
};

const std::vector<Position> knight_directions = {
        Position(-2, -1),
        Position(-2, 1),
        Position(-1, -2),
        Position(-1, 2),
        Position(1, -2),
        Position(1, 2),
        Position(2, -1),
        Position(2, 1)
};

bool Rules::checked(const Board &board, const Position &pos, char color) {
    Validate validate;
    int row = pos.row_;
    int col = pos.col_;
    int row_cnt = 0, col_cnt = 0;
    bool checked = false;
    // Looks out for diagonal, row and column checks
    for (const auto &direction: move_directions) {
        row_cnt = row + direction.row_;
        col_cnt = col + direction.col_;
        Position position(row_cnt, col_cnt);
        while (validate.valid_position(position)) {
            Square square = board.squares_[position.row_][position.col_];
            if (square.piece_ != nullptr) {
                if (square.piece_->get_color() != color) {
                    // Check if opponent's piece can attack your king
                    std::vector<Position> *possible_moves = square.piece_->possible_moves();
                    for (const auto &item: *possible_moves) {
                        if (!validate.valid_position(item)) {
                            break;
                        }
                        Square block = board.squares_[item.row_][item.col_];
                        if (validate.is_empty(block) && item != pos) {
                            continue;
                        } else {
                            if (item == pos) {
                                // Attacking king
                                // If attack in column as pawn -> no check
                                if (tolower(square.piece_->get_piece()) == 'p'
                                    && square.piece_->get_position().col_ == pos.col_) {
                                    checked = false;
                                } else {
                                    checked = true;
                                    break;
                                }
                            }
                        }
                    }
                    break;
                }
                break;
            }
            // Square was empty, look at next
            position.row_ += direction.row_;
            position.col_ += direction.col_;
        }
    }

    // Looks out for knight checks
    row_cnt = 0, col_cnt = 0;
    for (const auto &knightDirection: knight_directions) {
        row_cnt = row + knightDirection.row_;
        col_cnt = col + knightDirection.col_;
        Position position(row_cnt, col_cnt);
        if (validate.valid_position(position)) {
            Square square = board.squares_[position.row_][position.col_];
            if (!validate.is_empty(square)
                && tolower(square.piece_->get_piece()) == 'n'
                && !validate.is_same_color(square, color)) {
                checked = true;
                break;
            }
        }
    }
    return checked;
}

bool Rules::checkmate(Board &board, Player &player) {
    Move move;
    Validate validate;
    Position start_save = player.start_;
    Position end_save = player.end_;
    Position king(player.king_);
    bool castle = player.castling_;
    bool en_passant = player.en_passant_;
    std::vector<Position> pieces_pos_save = player.pieces_pos_;
    // Iterate over all pieces of the player
    for (const auto &piece_pos: player.pieces_pos_) {
        Square square = board.squares_[piece_pos.row_][piece_pos.col_];
        // Iterate over all possible moves for the piece
        std::vector<Position> *possible_moves = square.piece_->possible_moves();
        for (const auto &moves: *possible_moves) {
            // Validate pos
            if (!validate.valid_position(moves)) {
                continue;
            }
            player.start_ = piece_pos;
            player.end_ = moves;
            // Validate player's move
            if (!validate.validate_move(board, player, false)) {
                continue;
            }
            move.make_move(board, player);
            Position pos(player.king_.row_, player.king_.col_);
            // Check if the king is still in check after the moves
            if (!checked(board, pos, player.color_)) {
                player.castling_ = castle;
                player.en_passant_ = en_passant;
                player.king_ = king;
                player.pieces_pos_ = pieces_pos_save;

                move.undo_move(board, player);

                player.start_ = start_save;
                player.end_ = end_save;
                // King can escape check, not in checkmate
                return false;
            }
            player.castling_ = castle;
            player.en_passant_ = en_passant;
            player.king_ = king;
            player.pieces_pos_ = pieces_pos_save;
            move.undo_move(board, player);
            break;
        }
    }
    return true;
}

bool Rules::stalemate(Board &board, Player &player) {
    Validate validate;
    // Check if the player has any legal moves
    for (const auto &pos: player.pieces_pos_) {
        Square &square = board.squares_[pos.row_][pos.col_];
        std::vector<Position> *possible_moves = square.piece_->possible_moves();
        for (const auto &move: *possible_moves) {
            if (!validate.valid_position(move)) {
                continue;
            }
            Position start = player.start_;
            Position end = player.end_;
            player.start_ = pos;
            player.end_ = move;
            // At least one legal move
            if (validate.validate_move(board, player, false)) {
                if (tolower(square.piece_->get_piece()) == 'k') {
                    if (checked(board, move, player.color_)) {
                        continue;
                    }
                }
                // Legal move found
                player.start_ = start;
                player.end_ = end;
                return false;
            }
            player.start_ = start;
            player.end_ = end;
        }
    }
    // No legal move found
    return true;
}