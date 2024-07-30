#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "Player.h"
#include "Move.h"

class HumanPlayer : public Player {
public:
    Move make_move();
};

#endif