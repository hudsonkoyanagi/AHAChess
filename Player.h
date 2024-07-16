#ifndef PLAYER_H
#define PLAYER_H
#include "Move.h"

class Player {
public:
    virtual Move get_move() = 0;
};

#endif