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

    Piece(char l, Cord c, COLOURS col, PIECES t, bool b =false);
    void set_empty();
    bool is_empty();

    int start_row(); // returns the INDEX of its starting point
};

#endif