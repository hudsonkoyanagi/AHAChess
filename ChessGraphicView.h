#ifndef CHESS_GRAPHIC_VIEW_H
#define CHESS_GRAPHIC_VIEW_H
#include "ChessView.h"

class Xwindow;

class ChessGraphicView : public ChessView {
private:
    Xwindow* win;
    int side_length; // should probably be 0 mod 8
 public:
  void render();
  void update(Piece* a, Piece* b);
};

#endif