#ifndef CHESS_CONTROLLER_H
#define CHESS_CONTROLLER_H
#include "ChessModel.h"
#include "Player.h"

class ChessController {
private:
    ChessModel* model;
    float black_wins = 0;
    float white_wins = 0;

    bool white_to_move = true;

    Player* p1 = nullptr; // nullptr if human player
    Player* p2 = nullptr; // nullptr if human player
public:
    void input_loop();
    void game_loop();
    void setup_loop();

    ChessController(ChessModel* m) : model{m} {}
};

#endif