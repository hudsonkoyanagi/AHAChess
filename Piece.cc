#include "Piece.h"

Piece::Piece(Cord c, COLOURS col, PIECES t, bool b ) : loc{ c }, col{ col }, type{ t }, has_moved{ b } {}

void Piece::set_empty() {
    type = EMPTY;
    col = NONE;
}

bool Piece::is_empty() {
    return type == EMPTY && col == NONE;
}

int Piece::start_row() {
    if(this->is_empty()) return -1;

    if(col == WHITE) {
        if(type == PAWN) return 6;
        else return 7;
    }

    if (type == PAWN) return 1;
    else return 0;


}