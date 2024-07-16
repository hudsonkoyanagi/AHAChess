#ifndef CHESS_GRAPHIC_VIEW_H
#define CHESS_GRAPHIC_VIEW_H

#include "ChessView.h"
#include "window.h"

class ChessGraphicView : public ChessView {
  private:
    Xwindow win;
    const int side_length; // should probably be 0 mod 8
    const int tile_length;
  public:
    ChessGraphicView(int size = 800);
    void render();
    void update(Piece* a, Piece* b);
};
#endif