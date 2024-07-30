#include "ChessTextView.h"
#include <iostream>
#include "Piece.h"
#include "utils.h"

void ChessTextView::render() {
  std::cout << "- +-------------------------------+" << std::endl;

  for (int r = 0; r < 8; r++) {
    std::cout << 8 - r << " " << "|";
    for (int c = 0; c < 8; c++) {
      std::cout << " " << board[r][c] << " |";
    }
    std::cout << std::endl << "- +-------------------------------+" << std::endl;
  }
  std::cout << "  | a | b | c | d | e | f | g | h |" << std::endl << std::endl;
}

void ChessTextView::render(Piece* a, Cord a_cord, Piece* b, Cord b_cord) {
  board[a_cord.row][a_cord.col] = piece_col_to_char(a->type, a->col);
  board[b_cord.row][b_cord.col] = piece_col_to_char(b->type, b->col);
  this->render();
}

void ChessTextView::render(const std::array<std::array<Piece*, 8>, 8>& b) {
  this->init_board(b);
  this->render();
}