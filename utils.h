#ifndef UTILS_H
#define UTILS_H
#include "Cord.h"
#include "Enums.h"
#include <string>

bool is_valid_cord(const std::string& str);
Cord str_to_cord(const std::string& str);

void printPiece(PIECES piece) ;
    void printMoveResult(MOVE_RESULTS result) ;
        void printColour(COLOURS colour) ;
#endif