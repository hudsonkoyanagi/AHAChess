#include "ChessModel.h"
#include "Enums.h"
#include "Cord.h"
#include "Piece.h"
#include "cmath"
#include "utils.h"


MOVE_RESULTS is_valid_pawn_move(Move m, const std::array<std::array<Piece*, 8>, 8>& board,  bool white_to_move) {
    Piece* p = board[m.start.row][m.start.col];
    Piece* target = board[m.end.row][m.end.col];
    if ((p->col == WHITE && !white_to_move) || (p->col == BLACK && white_to_move)) return INVALID_MOVE;    // correct colour check
    if (!(m.start.row == m.end.row || m.start.col == m.end.col)) return INVALID_MOVE;                      // piece must move


    if (m.start.col != m.end.col) { // capture/en pesant
        if (abs(m.start.row - m.end.row) != 1) return INVALID_MOVE;                // move one forward
        if (abs(m.start.col - m.end.col) != 1) return INVALID_MOVE;                // move one sideways
        if ((p->col == WHITE) && (m.start.row <= m.end.row)) return INVALID_MOVE;  // backwards move
        if ((p->col == BLACK) && (m.start.row >= m.end.row)) return INVALID_MOVE;  // backwards move
        if (target->type == EMPTY) return INVALID_MOVE;                            // no piece TODO: EN PASSANT
        if (target->col == p->col) return INVALID_MOVE;                            // same colour capture
        if (target->type == KING) return INVALID_MOVE;                             // can't capture king (TODO game should be over here anyway)
        return CAPTURE;
    }
    if (abs(m.start.row - m.end.row) == 2) { // starting square 
        if ((p->col == WHITE) && (m.start.row != 6) && (m.end.row != 4)) return INVALID_MOVE;  // must be on starting square
        if ((p->col == BLACK) && (m.start.row != 1) && (m.end.row != 3)) return INVALID_MOVE;  // must be on starting square
        if (target->type != EMPTY) return INVALID_MOVE;                                        // blocked
        if ((p->col == WHITE) && (board[5][m.start.col]->type != EMPTY)) return INVALID_MOVE;  // blocked
        if ((p->col == BLACK) && (board[2][m.start.col]->type != EMPTY)) return INVALID_MOVE;  // blocked
        return SUCCESS;
    }

    if (abs(m.start.row - m.end.row) == 1) { // forward move, possibly promotion
        if ((p->col == WHITE) && (m.start.row <= m.end.row)) return INVALID_MOVE;
        if ((p->col == BLACK) && (m.start.row >= m.end.row)) return INVALID_MOVE;
        if (target->type != EMPTY) return INVALID_MOVE;
        
        if ((p->col == WHITE) && (m.end.row == 0)) return PROMOTION;
        if ((p->col == BLACK) && (m.end.row == 7)) return PROMOTION;

        return SUCCESS;
    }

    return INVALID_MOVE;
}

MOVE_RESULTS is_valid_king_move(Move m, const std::array<std::array<Piece*, 8>, 8>& board, bool white_to_move) {
    // if moving 2 columnwise, then must be a castle
    if (abs(m.end.row - m.start.row) == 0 && abs(m.end.col - m.start.col) == 2) {
        if (p->has_moved) return INVALID_MOVE; // king can't have moved

        Piece* should_be_rook;
        int row = 0; // chess notation row not index
        if (p->col == WHITE) {
            row = 1;
            if (m.start.col > m.start.col) should_be_rook = at("a1");
            if (m.start.col < m.start.col) should_be_rook = at("h1");
        } else { // BLACK
            row = 8;
            if (m.start.col > m.start.col) should_be_rook = at("a8");
            if (m.start.col < m.start.col) should_be_rook = at("h8");
        }

        if (should_be_rook->col != p->col) return INVALID_MOVE;
        if (should_be_rook->type != ROOK) return INVALID_MOVE;
        if (should_be_rook->has_moved)    return INVALID_MOVE;

        if (m.start.col > m.start.col) { // Queen side castle
            if (!(at("b" + row)->is_empty()) || !(at("c" + row)->is_empty()) || !(at("d" + row)->is_empty())) return INVALID_MOVE;
            do_move(m); // move the king
            std::swap(board[m.start.row][3]->loc, board[m.start.row][0]->loc);
            std::swap(board[m.start.row][3], board[m.start.row][0]);

        } else { // King side castle
            if (!(at("f" + row)->is_empty()) || !(at("g" + row)->is_empty())) return INVALID_MOVE;
            do_move(m); // move the king
            std::swap(board[m.start.row][5]->loc, board[m.start.row][7]->loc);
            std::swap(board[m.start.row][5], board[m.start.row][7]);
        }

        return SUCCESS;
    }

    // if moving in a direction not 1 or 0, then invalid
    else if (abs(m.end.row - m.start.row) <= 1 && abs(m.end.col - m.start.col) <= 1) {
        if (!(target->type == EMPTY)) {
            if (target->col == p->col) return INVALID_MOVE; // can't capture own piece
            do_capture(m);
            return CAPTURE;
        } else {
            do_move(m);
            return SUCCESS;
        }
    }
    return INVALID_MOVE;
}