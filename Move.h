#ifndef MOVE_H
#define MOVE_H
#include "Cord.h"
#include "Enums.h"

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
    bool had_moved_prior = false;
    bool taken_had_moved_prior = false;

    Move(Cord s, Cord e);
    Move(int sr, int sc, int er, int ec);

    void print();
};

#endif