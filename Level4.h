#ifndef LEVEL_4_H
#define LEVEL_4_H
#include "ComputerPlayer.h"
#include "Move.h"

class Level4 : public ComputerPlayer {
public:
  Level4(ChessModel* model) : ComputerPlayer(model) {}
  ATTEMPT_RESULT make_move(bool white_to_move) override;
};

#endif