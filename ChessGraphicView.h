#ifndef CHESS_GRAPHIC_VIEW_H
#define CHESS_GRAPHIC_VIEW_H

#include "ChessView.h"
#include "window.h"

class ChessGraphicView : public ChessView {
private:
    Xwindow* win;

    const int side_length = 800;  // should probably be 0 mod 8
    const int tile_length = side_length / 8;

   public:
    void render();
    void update(Piece* a, Piece* b);
};

#endif