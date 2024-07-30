#include "Level1.h"
#include "ComputerPlayer.h"
#include "Enums.h"
#include <cstdlib> 
#include "Move.h"
#include <iostream>

//random legal moves that will be checked by randomly picking a piece and picking a random possible location
ATTEMPT_RESULT Level1::make_move(bool white_to_move) {
    std::cout << "Computer 1 making move\n";
    //iterate through board and then for each we see if
    std::vector<Cord> temp;
    std::vector<Move> moves;
    for (int i = 0;i < 8;++i) {
        for (int j = 0;j < 8;++j) {
            Piece* currPiece = model->board[i][j];
            temp.clear();
            if (currPiece->col == WHITE && white_to_move) {
                temp = model->get_all_valid_end_cords(Cord{i,j},white_to_move);
                for(auto c: temp){
                    std::pair<ATTEMPT_RESULT,Move> p = model->attempt_move_no_commit(Cord{i,j},c,white_to_move);
                    ATTEMPT_RESULT ar = p.first;
                    Move mv = p.second;
                    if(ar != FAILURE){
                        moves.push_back(mv);
                    }
                }
            } else if (currPiece->col == BLACK && !white_to_move) {
                temp = model->get_all_valid_end_cords(Cord{i,j},!white_to_move);
                for(auto c: temp){
                    std::pair<ATTEMPT_RESULT,Move> p = model->attempt_move_no_commit(Cord{i,j},c,!white_to_move);
                    ATTEMPT_RESULT ar = p.first;
                    Move mv = p.second;
                    if(ar != FAILURE){
                        moves.push_back(mv);
                    }
                }
            }
        }
    }
    while (!moves.empty()) {
        int randIndex = std::rand() % moves.size();
        ATTEMPT_RESULT validAttempt = model->attempt_move(moves[randIndex].start,moves[randIndex].end,white_to_move,true);
        if(validAttempt == FAILURE){
            moves.erase(moves.begin() + randIndex);
        }
        else{
            return validAttempt;
        }
    }
    return FAILURE; // something went wrong
}
