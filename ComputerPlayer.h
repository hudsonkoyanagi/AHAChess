#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H
#include "Player.h"
#include "ChessModel.h"

class ComputerPlayer {
protected:
    ChessModel* model;
public:
    ComputerPlayer(ChessModel* model) : model(model) {}
    virtual ~ComputerPlayer() {}
    virtual Move make_move(bool white_to_move) = 0;
};

#endif
