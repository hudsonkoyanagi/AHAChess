#ifndef LEVEL_1_H
#define LEVEL_1_H
#include "ComputerPlayer.h"
#include "Move.h"

class Level1 : public ComputerPlayer {
public:
  Level1(ChessModel* model) : ComputerPlayer(model) {}
  ATTEMPT_RESULT make_move(bool white_to_move) override;
};

#endif