#ifndef LEVEL_2_H
#define LEVEL_2_H
#include "ComputerPlayer.h"
#include "Move.h"

class Level2 : public ComputerPlayer {
public:
  Level2(ChessModel* model) : ComputerPlayer(model) {}
  ATTEMPT_RESULT make_move(bool white_to_move) override;
};

#endif