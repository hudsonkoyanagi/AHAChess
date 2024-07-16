#include "ChessGraphicView.h"
#include <iostream>

ChessGraphicView::ChessGraphicView(int size): side_length{size}, tile_length{size/8}, win{size, size} {
  std::string s;
  for (int r=0;r<8;++r) {
    for (int c=0;c<8;++c) {
      if ((r+c) % 2 == 0) {
        win.fillRectangle(tile_length*c, tile_length*r, tile_length, tile_length, Xwindow::White);
      }
      else {
        win.fillRectangle(tile_length*c, tile_length*r, tile_length, tile_length, Xwindow::Green);
      }

      if (board[r][c] != ' ') {
        s = board[r][c];
        win.drawString(tile_length*c + tile_length/2, tile_length*r + tile_length/2, s);
      }
    }
  }
}

void ChessGraphicView::render() {
  
}