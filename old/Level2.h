#ifndef LEVEL_2_H
#define LEVEL_2_H
#include "ComputerPlayer.h"
#include "Move.h"

class Level2 : public ComputerPlayer {
public:
  Level2(ChessModel* model) : ComputerPlayer(model) {}
  MOVE_RESULTS make_move(bool white_to_move) override;
};

#endif