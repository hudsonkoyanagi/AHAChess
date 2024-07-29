#ifndef PLAYER_H
#define PLAYER_H
#include "Move.h"

class Player {
public:
    virtual Move make_move(bool) = 0;
};

#endif