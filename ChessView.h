#ifndef CHESS_MODEL_H
#define CHESS_MODEL_H
#include <array>

class Piece;

class ChessView {
private:
    std::array<std::array<char, 8>, 8> board;
public: 
    void init_board(std::array<std::array<Piece*, 8>, 8> b); // sets the state of the whole board
    void update(Piece* a, int xa, int ya, Piece* b, int xb, int yb); // Updates on a move - only two tiles may change
    virtual void render() = 0;
};

#endif