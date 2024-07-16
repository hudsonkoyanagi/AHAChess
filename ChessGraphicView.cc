#include "ChessGraphicView.h"
#include <iostream>

void ChessGraphicView::render() {
  Xwindow win{side_length, side_length};
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