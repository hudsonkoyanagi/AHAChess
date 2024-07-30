#include "Level4.h"
#include "ComputerPlayer.h"
#include "Enums.h"
#include <cstdlib>
#include "Move.h"
#include <iostream>
#include <algorithm>

// random legal moves that will be checked by randomly picking a piece and picking a random possible location

bool compareByScoreDesc(const std::pair<Move, int> &a, const std::pair<Move, int> &b)
{
    return a.second > b.second;
}

std::vector<std::pair<PIECES, int>> pieceVals = {
    {QUEEN, 9},
    {ROOK, 5},
    {BISHOP, 3},
    {KNIGHT, 3},
    {PAWN, 1},
    {KING, 0}};

int get_piece_val(PIECES pie)
{
    for (const auto &pair : pieceVals)
    {
        if (pair.first == pie)
        {
            return pair.second;
        }
    }
    return 0;
}

ATTEMPT_RESULT Level4::make_move(bool white_to_move)
{
    std::cout << "Computer 4 making move\n";
    // iterate through board and then for each we see if
    //  std::vector<std::pair<Cord,Cord> > valid_attempts;
    //  std::vector<ATTEMPT_RESULT> checks;
    std::vector<Move> avoid_capture_moves;
    std::vector<Move> check_moves;
    std::vector<Move> capture_moves;
    std::vector<Move> standard_moves;
    std::vector<Cord> threatened_locs;
    std::vector<Cord> temp;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            Piece *currPiece = model->board[i][j];
            temp.clear();
            if (currPiece->col == WHITE && white_to_move)
            {
                temp = model->get_all_valid_end_cords(Cord{i, j}, white_to_move);
                for (auto c : temp)
                {
                    std::pair<ATTEMPT_RESULT, Move> p = model->attempt_move_no_commit(Cord{i, j}, c, white_to_move);
                    ATTEMPT_RESULT ar = p.first;
                    Move mv = p.second;
                    if (ar != FAILURE)
                    {
                        bool captureable = false;
                        for (int n = 0; n < 8; ++n)
                        {
                            for (int m = 0; m < 8; ++m)
                            {
                                Piece *q = model->board[n][m];
                                if (q->col == BLACK)
                                {
                                    std::vector<Cord> temp2 = model->get_all_valid_end_cords(Cord{n, m}, false);
                                    for (auto c2 : temp2)
                                    {
                                        if (c2.row == i && c2.col == j)
                                        {
                                            captureable = true;
                                        }
                                        threatened_locs.push_back(c2);
                                    }
                                }
                            }
                        }
                        if (captureable)
                        {
                            avoid_capture_moves.push_back(mv);
                        }
                        else if (mv.check)
                        {
                            check_moves.push_back(mv);
                        }
                        else if (mv.taken != EMPTY)
                        {
                            capture_moves.push_back(mv);
                        }
                        else
                        {
                            standard_moves.push_back(mv);
                        }
                    }
                }
            }
            else if (currPiece->col == BLACK && !white_to_move)
            {
                temp = model->get_all_valid_end_cords(Cord{i, j}, white_to_move);
                for (auto c : temp)
                {
                    std::pair<ATTEMPT_RESULT, Move> p = model->attempt_move_no_commit(Cord{i, j}, c, white_to_move);
                    ATTEMPT_RESULT ar = p.first;
                    Move mv = p.second;
                    if (ar != FAILURE)
                    {
                        bool captureable = false;
                        for (int n = 0; n < 8; ++n)
                        {
                            for (int m = 0; m < 8; ++m)
                            {
                                Piece *q = model->board[n][m];
                                if (q->col == BLACK)
                                {
                                    std::vector<Cord> temp2 = model->get_all_valid_end_cords(Cord{n, m}, true);
                                    for (auto c2 : temp2)
                                    {
                                        if (c2.row == i && c2.col == j)
                                        {
                                            captureable = true;
                                        }
                                        threatened_locs.push_back(c2);
                                    }
                                }
                            }
                        }
                        if (captureable)
                        {
                            avoid_capture_moves.push_back(mv);
                        }
                        else if (mv.check)
                        {
                            check_moves.push_back(mv);
                        }
                        else if (mv.taken != EMPTY)
                        {
                            capture_moves.push_back(mv);
                        }
                        else
                        {
                            standard_moves.push_back(mv);
                        }
                    }
                }
            }
        }
    }

    std::cout << avoid_capture_moves.size() << " " << check_moves.size() << " " << capture_moves.size() << " " << standard_moves.size() << std::endl;

    std::vector<std::pair<Move, int>> bestMoves; // avoid captures, checks, captures, then standard
    // score all of our current Vectors
    for (auto c : threatened_locs)
    {
        for (int i = 0; i < avoid_capture_moves.size();)
        {
            if (avoid_capture_moves[i].end.row == c.row && avoid_capture_moves[i].end.col == c.col)
            {
                avoid_capture_moves.erase(avoid_capture_moves.begin() + i);
            }
            else
            {
                i++;
            }
        }
    }
    int score = 0;
    for (auto it : avoid_capture_moves)
    {
        score = get_piece_val(model->board[it.start.row][it.start.col]->type);
        bestMoves.push_back(std::make_pair(it, score));
    }
    for (auto it : check_moves)
    {
        bestMoves.push_back(std::make_pair(it, 5)); // putting an opponent into check is worth 5
    }
    for (auto it : capture_moves)
    {
        score = get_piece_val(model->board[it.end.row][it.end.col]->type);
        bestMoves.push_back(std::make_pair(it, score));
    }
    for (auto it : standard_moves)
    {
        bestMoves.push_back(std::make_pair(it, 0));
    }
    std::sort(bestMoves.begin(), bestMoves.end(), compareByScoreDesc);
    for(int i = 0;i<bestMoves.size();i++){
        std::cout<<bestMoves[i].second<<std::endl;
    }
    if (!bestMoves.empty())
    {
        for (int i = 0; i < bestMoves.size(); i++)
        {
            ATTEMPT_RESULT validAttempt = model->attempt_move(bestMoves[i].first.start, bestMoves[i].first.end, white_to_move, true);
            if (validAttempt != FAILURE)
            {
                return validAttempt;
            }
        }
    }
    return FAILURE; // something went wrong
}