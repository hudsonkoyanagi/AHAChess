#include "Level1.h"
#include "ComputerPlayer.h"
#include "Enums.h"
#include <cstdlib> 
#include "Move.h"

//random legal moves that will be checked by randomly picking a piece and picking a random possible location
Move Level1::make_move(bool white_to_move) {
    //iterate through board and then for each we see if
    std::vector<Move> valid_moves;
    for (int i = 0;i < 8;++i) {
        for (int j = 0;j < 8;++j) {
            Piece* currPiece = model->board[i][j];
            if (currPiece->col == WHITE && white_to_move) {
                for (int m = 0;m < 8;++m) {
                    for (int n = 0;n < 8;++n) {
                        MOVE_RESULTS currMove = model->is_valid(Move{ Cord{i,j},Cord{m,n} }, white_to_move);
                        if (currMove != INVALID_MOVE) {
                            valid_moves.push_back(Move{ Cord{i,j},Cord{m,n} });
                        }
                    }
                }
            } else if (currPiece->col == BLACK && !white_to_move) {
                for (int m = 0;m < 8;++m) {
                    for (int n = 0;n < 8;++n) {
                        MOVE_RESULTS currMove = model->is_valid(Move{ Cord{i,j},Cord{m,n} }, white_to_move);
                        if (currMove != INVALID_MOVE) {
                            valid_moves.push_back(Move{ Cord{i,j},Cord{m,n} });
                        }
                    }
                }
            }
        }
    }
    if (!valid_moves.empty()) {
        return valid_moves[std::rand() % valid_moves.size()];
    }
    return Move{ -1,-1,-1,-1 }; // something went wrong
}
