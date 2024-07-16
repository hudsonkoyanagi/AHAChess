#ifndef ENUMS_H
#define ENUMS_H

enum COLOURS {
    BLACK,
    WHITE,
    NONE
};

enum MOVE_RESULTS {
    BLACK_IN_CHECK, 
    WHITE_IN_CHECK, 
    BLACK_CHECKMATED, 
    WHITE_CHECKMATED, 
    INVALID_MOVE, 
    SUCCESS
};

enum PIECES {
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN,
    EMPTY
};

#endif