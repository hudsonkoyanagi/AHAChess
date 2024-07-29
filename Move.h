#ifndef MOVE_H
#define MOVE_H
#include "Cord.h"

class Move {
    public:
    // bool is_resign = false;
    Cord start;
    Cord end;

    // Move flags
    PIECES moved;
    PIECES taken;
    MOVE_RESULTS move_result;

    bool check = false;
    
    Move(Cord s, Cord e);
};

#endif