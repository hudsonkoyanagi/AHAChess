#include "Piece.h"

Piece::Piece(char l, Cord c, COLOURS col, PIECES t, bool b ) : letter{ l }, loc{ c }, col{ col }, type{ t }, has_moved{ b } {}

void Piece::set_empty() {
    type = EMPTY;
    col = NONE;
    letter = ' ';
}

bool Piece::is_empty() {
    return type == EMPTY && col == NONE && letter == ' ';
}