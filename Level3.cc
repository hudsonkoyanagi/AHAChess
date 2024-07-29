#include "Level3.h"
#include "ComputerPlayer.h"
#include "Enums.h"
#include <cstdlib> 
#include "Move.h"
#include <iostream>


//priority: if currently in check try to get out of check>if can be captured get out of capture>most point captures>aggressively capture others
MOVE_RESULTS Level3::make_move(bool white_to_move) {
    std::cout << "Computer 3 making move\n";
    //iterate through board and then for each we see if
    std::vector<Move> valid_moves;
    std::vector<Move> valid_checks;
    std::vector<Move> valid_captures;
    
    std::vector<Move> opponent_checks;
    std::vector<Move> opponent_captures;

    //iterate thru opponent's pieces and see if any result in checks
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
                            if(currMove == CAPTURE || CAPTURE_WITH_CHECK){
                                opponent_captures.push_back(mv);
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
                            if(currMove == CAPTURE || CAPTURE_WITH_CHECK){
                                opponent_captures.push_back(mv);
                            }
                        }
                    }
                }
            }
        }
    }
    //if we are currently in check then get out of check
    COLOURS currColour = white_to_move ? WHITE : BLACK;
    if(model->is_in_check(currColour)){
        for (int i = 0;i < 8;++i) {
            for (int j = 0;j < 8;++j) {
                Piece* currPiece = model->board[i][j];
                if (currPiece->col == WHITE && white_to_move) {
                    for (int m = 0; m < 8;++m) {
                        for (int n = 0;n < 8;++n) {
                            std::cout << "";
                            MOVE_RESULTS currMove = model->is_valid(Move{Cord{i,j},Cord{m,n} }, white_to_move);
                            if (currMove != INVALID_MOVE) {
                                Move mv{ Cord{i,j}, Cord{m,n} };
                                mv.move_result = currMove;
                                model->make_move(mv,white_to_move);
                                if(model->is_in_check(currPiece->col)){
                                    model->undo_move(mv);
                                }
                                else{
                                    return mv.move_result;
                                }
                            }
                        }
                    }
                }  
            }
        }
    }
    //if we get to here then we check for captures
    Cord piecePos =  Cord{-1,-1};
    for(int i = 0;i<opponent_captures.size();i++){
        piecePos = opponent_captures[i].end; //this will point to the piece on my side that is being captured
        if(model->board[piecePos.row][piecePos.col]->type == QUEEN){
            //move the queen out of the way, I think the better way of doing this is iterating thru queen to see if any one of the pieces on the other side can take it
        }
    }
    //prioritize checks>captures>aggressive taking
    // std::cout<<"Why are you invalid"<<std::endl;
    return INVALID_MOVE; // something went wrong
}
