#ifndef CHESS_VIEW_H
#define CHESS_VIEW_H
#include <array>
#include "Cord.h"
#include "Piece.h"


class ChessView {
protected:
    std::array<std::array<char, 8>, 8> board;
public:

    virtual ~ChessView() {};
 // sets the state of the whole board
 void init_board(const std::array<std::array<Piece*, 8>, 8>& b);

 // renders based on internal state only
 virtual void render() = 0;
 // renders after a single move
 virtual void render(Piece* a, Cord a_cord, Piece* b, Cord b_cord) = 0;
 // renders an entire new board state
 virtual void render(const std::array<std::array<Piece*, 8>, 8>& b) = 0;
};

#endif