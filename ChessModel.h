#ifndef CHESS_MODEL_H
#define CHESS_MODEL_H
#include <array>
#include <vector>
#include <iostream>

#include "Cord.h"
#include "Enums.h"
#include "Piece.h"
#include "Move.h"
#include "ChessTextView.h"

class ChessModel {
private:
    std::array<std::array<Piece*, 8>, 8> board;
    std::vector<Move> history;
    std::array<char, 8> white_pieces;
    std::array<char, 8> black_pieces;

    bool white_to_move = true;
    bool white_in_check = false;
    bool black_in_check = false;
    bool white_in_mate = false;
    bool black_in_mate = false;

    Cord pawn_to_promote;

    ChessTextView* view;

    void reset();
    void empty();

public:
    ChessModel(ChessTextView* v);
    ~ChessModel();


    MOVE_RESULTS make_move(Move m);

    void setup_start(); // empties board
    void setup_add_piece(char piece, Cord move);
    void setup_rem_piece(Cord move);
    void setup_set_turn(COLOURS col);
    bool setup_finish(); // returns false if setup cannot be finished (invalid pawns, kings, etc)

    // friend std::istream& operator>>(std::istream& is, ChessModel& b);
    // friend std::ostream& operator<<(std::ostream& os, const ChessModel& b);
    friend class ComputerPlayer;
};

#endif