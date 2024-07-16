#ifndef LEVEL_2_H
#define LEVEL_2_H
#include "ComputerPlayer.h"
#include "Move.h"

class Level2 : public ComputerPlayer {
 private:
  // smart algo things
 public:
  Move make_move();
};

#endif