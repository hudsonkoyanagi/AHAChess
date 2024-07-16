#ifndef LEVEL_1_H
#define LEVEL_1_H
#include "ComputerPlayer.h"
#include "Move.h"

class Level1 : public ComputerPlayer {
 private:
  // smart algo things
 public:
  Move make_move();
};

#endif