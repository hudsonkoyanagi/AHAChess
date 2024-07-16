#ifndef CHESS_CONTROLLER_H
#define CHESS_CONTROLLER_H
#include "ChessModel.h"

class ChessController {
private:
    ChessModel* model;
    int black_wins = 0;
    int white_wins = 0;
public:
    void input_loop();
    void game_loop();

    ChessController(ChessModel* m) : model{m} {}
};

#endif