#ifndef CHESS_CONTROLLER_H
#define CHESS_CONTROLLER_H
#include "ChessModel.h"

class ChessController {
private:
    ChessModel* model;
public:
    void input_loop();
};

#endif