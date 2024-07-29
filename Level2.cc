#include "Level2.h"
#include "ComputerPlayer.h"
#include "Enums.h"
#include <cstdlib> 
#include "Move.h"
#include <iostream>

//random legal moves that will be checked by randomly picking a piece and picking a random possible location
MOVE_RESULTS Level2::make_move(bool white_to_move) {
    std::cout << "Computer 2 making move\n";
    //iterate through board and then for each we see if
    std::vector<Move> valid_moves;
    std::vector<Move> valid_checks;
    std::vector<Move> valid_captures;
    for (int i = 0;i < 8;++i) {
        for (int j = 0;j < 8;++j) {
            Piece* currPiece = model->board[i][j];
            if (currPiece->col == WHITE && white_to_move) {
                for (int m = 0; m < 8;++m) {
                    for (int n = 0;n < 8;++n) {
                        std::cout << "";
                        MOVE_RESULTS currMove = model->is_valid(Move{ Cord{i,j},Cord{m,n} }, white_to_move);
                        if (currMove != INVALID_MOVE) {
                            Move mv{ Cord{i,j}, Cord{m,n} };
                            mv.move_result = currMove;
                            if(currMove == CAPTURE){
                                valid_captures.push_back(mv);
                            }
                            else if(currMove == CAPTURE_WITH_CHECK || currMove == PROMOTE_WITH_CHECK || currMove == MOVE_WITH_CHECK || currMove  == CASTLE_WITH_CHECK || currMove == EN_PASSANT_WITH_CHECK){
                                valid_checks.push_back(mv);
                            }
                            else{
                                valid_moves.push_back(mv);
                            }
                        }
                    }
                }
            } else if (currPiece->col == BLACK && !white_to_move) {
                for (int m = 0; m < 8;++m) {
                    for (int n = 0;n < 8;++n) {
                        std::cout << "";
                        MOVE_RESULTS currMove = model->is_valid(Move{ Cord{i,j},Cord{m,n} }, white_to_move);
                        if (currMove != INVALID_MOVE) {
                            Move mv{ Cord{i,j}, Cord{m,n} };
                            mv.move_result = currMove;
                            if(currMove == CAPTURE){
                                valid_captures.push_back(mv);
                            }
                            else if(currMove == CAPTURE_WITH_CHECK || currMove == PROMOTE_WITH_CHECK || currMove == MOVE_WITH_CHECK || currMove  == CASTLE_WITH_CHECK || currMove == EN_PASSANT_WITH_CHECK){
                                valid_checks.push_back(mv);
                            }
                            else{
                                valid_moves.push_back(mv);
                            }
                        }
                    }
                }
            }
        }
    }
    //prioritize checks>captures>random moves
    if(!valid_checks.empty()){
        Move m = valid_checks[std::rand() % valid_checks.size()];
        model->make_move(m, white_to_move);
        return m.move_result;
    }
    else if(!valid_captures.empty()){
        Move m = valid_captures[std::rand() % valid_captures.size()];
        model->make_move(m, white_to_move);
        return m.move_result;
    }
    else if (!valid_moves.empty()) {
        Move m = valid_moves[std::rand() % valid_moves.size()];
        model->make_move(m, white_to_move);
        return m.move_result;
    }
    // std::cout<<"Why are you invalid"<<std::endl;
    return INVALID_MOVE; // something went wrong
}