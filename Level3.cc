#include "Level3.h"
#include "ComputerPlayer.h"
#include "Enums.h"
#include <cstdlib> 
#include "Move.h"
#include <iostream>

//random legal moves that will be checked by randomly picking a piece and picking a random possible location
ATTEMPT_RESULT Level3::make_move(bool white_to_move) {
    std::cout << "Computer 3 making move\n";
    //iterate through board and then for each we see if
    // std::vector<std::pair<Cord,Cord> > valid_attempts;
    // std::vector<ATTEMPT_RESULT> checks;
    std::vector<Move> avoid_capture_moves;
    std::vector<Move> check_moves;
    std::vector<Move> capture_moves;
    std::vector<Move> standard_moves;
    std::vector<Cord> threatened_locs;
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
                        bool captureable = false;
                        for (int n=0;n<8;++n) {
                            for (int m=0;m<8;++m) {
                                Piece* q = model->board[n][m];
                                if (q->col == BLACK) {
                                    std::vector<Cord> temp2 = model->get_all_valid_end_cords(Cord{n,m},false);
                                    for (auto c2 : temp2) {
                                        if (c2.row == i && c2.col == j) {
                                            captureable = true;
                                        }
                                        threatened_locs.push_back(c2);
                                    }
                                }
                            }
                        }
                        if (captureable) {
                            avoid_capture_moves.push_back(mv);
                        }
                        else if(mv.check){
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
                temp = model->get_all_valid_end_cords(Cord{i,j},white_to_move);
                for(auto c: temp){
                    std::pair<ATTEMPT_RESULT,Move> p = model->attempt_move_no_commit(Cord{i,j},c,white_to_move);
                    ATTEMPT_RESULT ar = p.first;
                    Move mv = p.second;
                    if(ar != FAILURE){
                        bool captureable = false;
                        for (int n=0;n<8;++n) {
                            for (int m=0;m<8;++m) {
                                Piece* q = model->board[n][m];
                                if (q->col == BLACK) {
                                    std::vector<Cord> temp2 = model->get_all_valid_end_cords(Cord{n,m},true);
                                    for (auto c2 : temp2) {
                                        if (c2.row == i && c2.col == j) {
                                            captureable = true;
                                        }
                                        threatened_locs.push_back(c2);
                                    }
                                }
                            }
                        }
                        if (captureable) {
                            avoid_capture_moves.push_back(mv);
                        }
                        else if(mv.check){
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

    std::cout << avoid_capture_moves.size() << " " << check_moves.size() << " " << capture_moves.size() << " " << standard_moves.size() << std::endl;

    //iterate thru avoid captures
    while(!avoid_capture_moves.empty()){
        int randIndex = std::rand() % avoid_capture_moves.size();
        bool skip = false;
        for (auto c3 : threatened_locs) {
            if (avoid_capture_moves[randIndex].end.row == c3.row && avoid_capture_moves[randIndex].end.col == c3.col) {
                avoid_capture_moves.erase(avoid_capture_moves.begin() + randIndex);
                skip = true;
            }
        }
        if (skip) continue;
        ATTEMPT_RESULT validAttempt = model->attempt_move(avoid_capture_moves[randIndex].start,avoid_capture_moves[randIndex].end,white_to_move,true);
        if(validAttempt == FAILURE){
            avoid_capture_moves.erase(avoid_capture_moves.begin() + randIndex);
        }
        else{
            return validAttempt;
        }
    }

    //iterate thru checks
    while(!check_moves.empty()){
        int randIndex = std::rand() % check_moves.size();
        ATTEMPT_RESULT validAttempt = model->attempt_move(check_moves[randIndex].start,check_moves[randIndex].end,white_to_move,true);
        if(validAttempt == FAILURE){
            check_moves.erase(check_moves.begin() + randIndex);
        }
        else{
            return validAttempt;
        }
    }
    
    while(!capture_moves.empty()){
        int randIndex = std::rand() % capture_moves.size();
        ATTEMPT_RESULT validAttempt = model->attempt_move(capture_moves[randIndex].start,capture_moves[randIndex].end,white_to_move,true);
        if(validAttempt == FAILURE){
            capture_moves.erase(capture_moves.begin() + randIndex);
        }
        else{
            return validAttempt;
        }
    }


    while(!standard_moves.empty()){
        int randIndex = std::rand() % standard_moves.size();
        ATTEMPT_RESULT validAttempt = model->attempt_move(standard_moves[randIndex].start,standard_moves[randIndex].end,white_to_move,true);
        if(validAttempt == FAILURE){
            standard_moves.erase(standard_moves.begin() + randIndex);
        }
        else{
            return validAttempt;
        }
    }
    return FAILURE; // something went wrong
}
