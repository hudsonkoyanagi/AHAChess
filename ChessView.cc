#include "ChessView.h"
#include "Piece.h"
#include <array>
#include "utils.h"


void ChessView::init_board(const std::array<std::array<Piece*, 8>, 8>& b) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board[i][j] = piece_col_to_char(b[i][j]->type, b[i][j]->col);
        }
    }
}
