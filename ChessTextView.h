#ifndef CHESS_TEXT_VIEW_H
#define CHESS_TEXT_VIEW_H
#include "ChessView.h"

class ChessTextView : public ChessView {
public:
    void render();
    void update(Piece* a, Piece* b);
};

#endif