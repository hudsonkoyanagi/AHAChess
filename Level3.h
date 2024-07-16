#ifndef LEVEL_3_H
#define LEVEL_3_H
#include "ComputerPlayer.h"
#include "Move.h"

class Level3 : public ComputerPlayer {
 private:
  // smart algo things
 public:
  Move make_move();
};

#endif