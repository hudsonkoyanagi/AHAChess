#ifndef CHESS_TEXT_VIEW_H
#define CHESS_TEXT_VIEW_H
#include "ChessView.h"

class ChessTextView : public ChessView {
public:

    // renders based on internal state only
    void render();
    // renders after a single move
    void render(Piece* a, Piece* b);  
    // renders an entire new board state
    void render(const std::array<std::array<Piece*, 8>, 8>& b);  
};

#endif