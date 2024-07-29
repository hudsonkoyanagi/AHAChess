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


void printColour(COLOURS colour) {
    std::cout << "COLOURS : ";
    switch (colour) {
    case BLACK: std::cout << "BLACK = " << BLACK; break;
    case WHITE: std::cout << "WHITE = " << WHITE; break;
    case NONE: std::cout << "NONE = " << NONE; break;
    }
    std::cout << std::endl;
}

// Function to print MOVE_RESULTS enum
void printMoveResult(MOVE_RESULTS result) {
    std::cout << "MOVE_RESULTS : ";
    switch (result) {
    case BLACK_CHECKMATED: std::cout << "BLACK_CHECKMATED = " << BLACK_CHECKMATED; break;
    case WHITE_CHECKMATED: std::cout << "WHITE_CHECKMATED = " << WHITE_CHECKMATED; break;
    case STALEMATE: std::cout << "STALEMATE = " << STALEMATE; break;
    case SUCCESS: std::cout << "SUCCESS = " << SUCCESS; break;
    case INVALID_MOVE: std::cout << "INVALID_MOVE = " << INVALID_MOVE; break;
    case CAPTURE: std::cout << "CAPTURE = " << CAPTURE; break;
    case PROMOTION: std::cout << "PROMOTION = " << PROMOTION; break;
    case CASTLE: std::cout << "CASTLE = " << CASTLE; break;
    case EN_PASSANT: std::cout << "EN_PASSANT = " << EN_PASSANT; break;
    case CAPTURE_WITH_CHECK: std::cout << "CAPTURE_WITH_CHECK = " << CAPTURE_WITH_CHECK; break;
    case PROMOTE_WITH_CHECK: std::cout << "PROMOTE_WITH_CHECK = " << PROMOTE_WITH_CHECK; break;
    case MOVE_WITH_CHECK: std::cout << "MOVE_WITH_CHECK = " << MOVE_WITH_CHECK; break;
    case CASTLE_WITH_CHECK: std::cout << "CASTLE_WITH_CHECK = " << CASTLE_WITH_CHECK; break;
    case EN_PASSANT_WITH_CHECK: std::cout << "EN_PASSANT_WITH_CHECK = " << EN_PASSANT_WITH_CHECK; break;
    }
    std::cout << std::endl;
}

// Function to print PIECES enum
void printPiece(PIECES piece) {
    std::cout << "PIECES : ";
    switch (piece) {
    case KING: std::cout << "KING = " << KING; break;
    case QUEEN: std::cout << "QUEEN = " << QUEEN; break;
    case ROOK: std::cout << "ROOK = " << ROOK; break;
    case BISHOP: std::cout << "BISHOP = " << BISHOP; break;
    case KNIGHT: std::cout << "KNIGHT = " << KNIGHT; break;
    case PAWN: std::cout << "PAWN = " << PAWN; break;
    case EMPTY: std::cout << "EMPTY = " << EMPTY; break;
    }
    std::cout << std::endl;
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