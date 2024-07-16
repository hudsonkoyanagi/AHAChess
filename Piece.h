#ifndef PIECE_H
#define PIECE_H

#include "Enums.h"
#include "Cord.h"

struct Piece {
    char letter;
    PIECES type;
    COLOURS col;
    bool has_moved;
    Cord loc;

    Piece(char l, Cord c, COLOURS col, PIECES t, bool b=false) : letter{l}, loc{c}, col{col}, type{t}, has_moved{b} {}
};

#endif