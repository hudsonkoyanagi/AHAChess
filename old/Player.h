#ifndef PLAYER_H
#define PLAYER_H
#include "Move.h"
#include "Enums.h"

class Player {
public:
    virtual MOVE_RESULTS make_move(bool) = 0;
};

#endif