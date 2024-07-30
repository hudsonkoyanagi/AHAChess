#include "utils.h"
#include "Cord.h"
#include <string>
#include <iostream>
#include "Enums.h"


bool is_valid_cord(const std::string& str) {
    return ((str.length() == 2) && ('a' <= str[0] && str[0] <= 'h') &&
        ('1' <= str[1] && str[1] <= '8'));
}

// Invalid cord is returned as <-1,-1>
// Returns row, col indexes in board from a chess cord string
Cord str_to_cord(const std::string& str) {
    if (!is_valid_cord(str)) return Cord{ -1, -1 };
    int row = 7 - (str[1] - '0' - 1);
    int col = str[0] - 'a';  // converts 'a'-'h' to index
    return Cord{ row, col };
}

char piece_col_to_char(PIECES p, COLOURS c) {
    char ch;

    if(p==KING) ch = 'k';
    else if(p==QUEEN) ch='q';
    else if (p == ROOK) ch = 'r';
    else if (p == BISHOP) ch = 'b';
    else if (p == KNIGHT) ch = 'n';
    else if (p == PAWN) ch = 'p';
    else if (p == EMPTY) ch = ' ';

    if(p != EMPTY && c == WHITE) ch -= 32;
    return ch;

}
