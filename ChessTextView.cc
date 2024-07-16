#include "ChessTextView.h"
#include <iostream>

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