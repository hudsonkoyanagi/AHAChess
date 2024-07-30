#ifndef CHESS_GRAPHIC_VIEW_H
#define CHESS_GRAPHIC_VIEW_H

#include "ChessView.h"
#include "window.h"
#include "Move.h"

class ChessGraphicView : public ChessView {
  private:
    Xwindow win;
    const int side_length; // should probably be 0 mod 8
    const int tile_length;
  public:
    ChessGraphicView(int size = 800);
    
    // renders based on internal state only
    void render();
    // renders after a single move
    void render(Piece* a, Cord a_cord, Piece* b, Cord b_cord);
    // renders an entire new board state
    void render(const std::array<std::array<Piece*, 8>, 8>& b);
};
#endif