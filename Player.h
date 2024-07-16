#ifndef PLAYER_H
#define PLAYER_H
#include "Move.h"

class Player {
public:
    virtual Move make_move() = 0;
};

#endif