#ifndef PIECE_H
#define PIECE_H

#include "Enums.h"
#include "Cord.h"

struct Piece {
    PIECES type;
    COLOURS col;
    bool has_moved;
    Cord loc;

    Piece(Cord c=Cord{-1,-1}, COLOURS col=NONE, PIECES t=EMPTY, bool b =false);
    void set_empty();
    bool is_empty();

    int start_row(); // returns the INDEX of its starting point
};

#endif