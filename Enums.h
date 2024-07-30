#ifndef ENUMS_H
#define ENUMS_H

enum COLOURS {
    BLACK,
    WHITE,
    NONE
};

enum MOVE_RESULTS {
    STANDARD,
    PROMOTION,
    CASTLE,
    EN_PASSANT,
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

enum ATTEMPT_RESULT {
    SUCCESS,
    FAILURE,
    BLACK_IN_CHECK,
    WHITE_IN_CHECK,
    BLACK_CHECKMATED,
    WHITE_CHECKMATED,
    STALEMATE
};

#endif
