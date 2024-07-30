#ifndef PLAYER_H
#define PLAYER_H
#include "Move.h"
#include "Enums.h"

class Player {
public:
    virtual ATTEMPT_RESULT make_move(bool) = 0;
};

#endif