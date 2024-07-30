#include "Move.h"
#include "utils.h"
#include <iostream>

Move::Move(Cord s, Cord e) : start{ s }, end{ e }, moved{ EMPTY }, taken{ EMPTY }, move_result{ INVALID_MOVE }, check{ false }, had_moved_prior{ false }, taken_had_moved_prior{ false } {}

Move::Move(int sr, int sc, int er, int ec) : start{ Cord{sr, sc} }, end{ Cord{er, ec} }, moved{ EMPTY }, taken{ EMPTY }, move_result{ INVALID_MOVE }, check{ false } {}


void Move::print() {
    std::cout << "start: <" << start.row << ", " << start.col << ">" << std::endl;
    std::cout << "end: <" << end.row << ", " << end.col << ">" << std::endl;
    printMoveResult(move_result);
    printPiece(moved);
    printPiece(taken);
}