#include "Level1.h"
#include "ComputerPlayer.h"
#include "Enums.h"
#include <cstdlib> 
#include "Move.h"
#include <iostream>

//random legal moves that will be checked by randomly picking a piece and picking a random possible location
Move Level1::make_move(bool white_to_move) {
    std::cout << "Computer 1 making move\n";
    //iterate through board and then for each we see if
    std::vector<Move> valid_moves;
    for (int i = 0;i < 8;++i) {
        for (int j = 0;j < 8;++j) {
            Piece* currPiece = model->board[i][j];
            if (currPiece->col == WHITE && white_to_move) {
                for (int m = 0; m < 8;++m) {
                    for (int n = 0;n < 8;++n) {
                        std::cout << "";
                        Move to_make{ Cord{i,j}, Cord{m,n} };
                        to_make.moved = model->board[i][j]->type;
                        to_make.taken = model->board[m][n]->type;
                        to_make.had_moved_prior = model->board[i][j]->has_moved;
                        to_make.taken_had_moved_prior = model->board[m][n]->has_moved;
                        Move currMove = model->is_valid(to_make, white_to_move);
                        if (currMove.move_result != INVALID_MOVE) {
                            valid_moves.push_back(currMove);
                        }
                    }
                }
            } else if (currPiece->col == BLACK && !white_to_move) {
                for (int m = 0;m < 8;++m) {
                    for (int n = 0;n < 8;++n) {
                        std::cout << "";
                        Move to_make{ Cord{i,j}, Cord{m,n} };
                        to_make.moved = model->board[i][j]->type;
                        to_make.taken = model->board[m][n]->type;
                        to_make.had_moved_prior = model->board[i][j]->has_moved;
                        to_make.taken_had_moved_prior = model->board[m][n]->has_moved;
                        Move currMove = model->is_valid(to_make, white_to_move);
                        if (currMove.move_result != INVALID_MOVE) {
                            valid_moves.push_back(currMove);
                        }
                    }
                }
            }
        }
    }
    std::cout<<"valid_moves"<<std::endl;
    for(int i = 0;i<valid_moves.size();i++){
        std::cout<<valid_moves[i].move_result<<std::endl;
    }
    if (!valid_moves.empty()) {
        Move m = valid_moves[std::rand() % valid_moves.size()];
        model->make_move(m, white_to_move);
        // std::cout<<"Move result from Level1.cc "<<m.move_result<<std::endl;
        return m.move_result;
    }
    // std::cout<<"Why are you invalid"<<std::endl;
    return INVALID_MOVE; // something went wrong
}
