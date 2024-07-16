#include "ChessView.h"
#include "Piece.h"
#include <array>


void ChessView::init_board(const std::array<std::array<Piece*, 8>, 8>& b) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board[i][j] = b[i][j]->letter;
        }
    }
}

void ChessView::update(Piece* a, Piece* b) {
    board[a->loc.row][a->loc.col] = a->letter;
    board[b->loc.row][b->loc.col] = b->letter;
}