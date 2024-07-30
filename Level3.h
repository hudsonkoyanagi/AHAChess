#ifndef LEVEL_3_H
#define LEVEL_3_H
#include "ComputerPlayer.h"
#include "Move.h"

class Level3 : public ComputerPlayer {
public:
  Level3(ChessModel* model) : ComputerPlayer(model) {}
  ATTEMPT_RESULT make_move(bool white_to_move) override;
};

#endif