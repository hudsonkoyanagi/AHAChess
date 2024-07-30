#include "Level1.h"
#include "ComputerPlayer.h"
#include "Enums.h"
#include <cstdlib> 
#include "Move.h"
#include <iostream>

//random legal moves that will be checked by randomly picking a piece and picking a random possible location

//so this will have to 
ATTEMPT_RESULT Level1::make_move(bool white_to_move) {
    std::cout << "Computer 1 making move\n";
    //iterate through board and then for each we see if
    std::vector<std::pair<Cord,Cord> > valid_attempts;
    std::vector<Cord> temp;
    for (int i = 0;i < 8;++i) {
        for (int j = 0;j < 8;++j) {
            Piece* currPiece = model->board[i][j];
            if (currPiece->col == WHITE && white_to_move) {
                temp = model->get_all_valid_end_cords(Cord{i,j},white_to_move);
                for(auto c: temp){
                    valid_attempts.push_back(std::make_pair(Cord{i,j},c));
                }
            } else if (currPiece->col == BLACK && !white_to_move) {
                temp = model->get_all_valid_end_cords(Cord{i,j},!white_to_move);
                for(auto c: temp){
                    valid_attempts.push_back(std::make_pair(Cord{i,j},c));
                }
            }
        }
    }
    // std::cout<<"valid_attempts"<<std::endl;
    // for(int i = 0;i<valid_attempts.size();i++){
    //     std::cout<<valid_attempts[i].first.row<<" "<<valid_attempts[i].second.row<<std::endl;
    // }
    while (!valid_attempts.empty()) {
        int randIndex = std::rand() % valid_attempts.size();
        std::pair<Cord,Cord> currPair = valid_attempts[randIndex];
        ATTEMPT_RESULT validAttempt = model->attempt_move(currPair.first,currPair.second,white_to_move);
        if(validAttempt == FAILURE){
            valid_attempts.erase(valid_attempts.begin() + randIndex);
        }
        else{
            return validAttempt;
        }
    }
    return FAILURE; // something went wrong
}
