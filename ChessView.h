#ifndef CHESS_VIEW_H
#define CHESS_VIEW_H
#include <array>

class Piece;

class ChessView {
protected:
    std::array<std::array<char, 8>, 8> board;
public: 
    void init_board(const std::array<std::array<Piece*, 8>, 8>& b); // sets the state of the whole board
    void update(Piece* a, Piece* b); // Updates on a move - only two tiles may change and pieces contain their location
    virtual void render() = 0;
};

#endif