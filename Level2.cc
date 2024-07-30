#include "Level2.h"
#include "ComputerPlayer.h"
#include "Enums.h"
#include <cstdlib> 
#include "Move.h"
#include <iostream>

//random legal moves that will be checked by randomly picking a piece and picking a random possible location
ATTEMPT_RESULT Level2::make_move(bool white_to_move) {
    std::cout << "Computer 2 making move\n";
    //iterate through board and then for each we see if
    // std::vector<std::pair<Cord,Cord> > valid_attempts;
    // std::vector<ATTEMPT_RESULT> checks;
    std::vector<Move> check_moves;
    std::vector<Move> capture_moves;
    std::vector<Move> standard_moves;
    std::vector<Cord> temp;
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
                        if(mv.check){
                            check_moves.push_back(mv);
                        }
                        else if(mv.taken != EMPTY){
                            capture_moves.push_back(mv);
                        }
                        else{
                            standard_moves.push_back(mv);
                        }
                    }
                }
            } else if (currPiece->col == BLACK && !white_to_move) {
                temp = model->get_all_valid_end_cords(Cord{i,j},!white_to_move);
                for(auto c: temp){
                    std::pair<ATTEMPT_RESULT,Move> p = model->attempt_move_no_commit(Cord{i,j},c,!white_to_move);
                    ATTEMPT_RESULT ar = p.first;
                    Move mv = p.second;
                    if(ar != FAILURE){
                        if(mv.check){
                            check_moves.push_back(mv);
                        }
                        else if(mv.taken != EMPTY){
                            capture_moves.push_back(mv);
                        }
                        else{
                            standard_moves.push_back(mv);
                        }
                    }
                }      
            }
        }
    }

    //iterate thru checks
    while(!check_moves.empty()){
        int randIndex = std::rand() % check_moves.size();
        ATTEMPT_RESULT validAttempt = model->attempt_move(check_moves[randIndex].start,check_moves[randIndex].start,white_to_move,true);
        if(validAttempt == FAILURE){
            check_moves.erase(check_moves.begin() + randIndex);
        }
        else{
            return validAttempt;
        }
    }
    
    while(!capture_moves.empty()){
        int randIndex = std::rand() % capture_moves.size();
        ATTEMPT_RESULT validAttempt = model->attempt_move(capture_moves[randIndex].start,capture_moves[randIndex].start,white_to_move,true);
        if(validAttempt == FAILURE){
            capture_moves.erase(capture_moves.begin() + randIndex);
        }
        else{
            return validAttempt;
        }
    }


    while(!standard_moves.empty()){
        int randIndex = std::rand() % standard_moves.size();
        ATTEMPT_RESULT validAttempt = model->attempt_move(standard_moves[randIndex].start,standard_moves[randIndex].start,white_to_move,true);
        if(validAttempt == FAILURE){
            standard_moves.erase(standard_moves.begin() + randIndex);
        }
        else{
            return validAttempt;
        }
    }
    return FAILURE; // something went wrong
}
