#ifndef CHESS_MODEL_H
#define CHESS_MODEL_H
#include <array>
#include <vector>
#include <iostream>

#include "Cord.h"
#include "Enums.h"
#include "Piece.h"
#include "Move.h"
#include "ChessView.h"


class ChessModel {
private:
    std::array<std::array<Piece*, 8>, 8> board;
    std::vector<Move> history;
    std::array<char, 8> white_pieces;
    std::array<char, 8> black_pieces;
    std::vector<ChessView*> views;


    bool white_in_check = false;
    bool black_in_check = false;
    bool white_in_mate = false;
    bool black_in_mate = false;

    Cord pawn_to_promote;
public:
    ChessModel();
    ~ChessModel();

    void reset();
    void empty();

    Piece* find_king(COLOURS king_col);      // finds the piece of the white king
    bool is_in_check(COLOURS king_col);     // checks if any piece is putting a king in check (param=true for search white, param=false for search black)

    bool is_stalemate_for(COLOURS curr_col);    // checks if WHITE or BLACK is in stalemate

    MOVE_RESULTS is_valid(Move m, bool white_to_move) ;
    MOVE_RESULTS make_move(Move m, bool white_to_move);
    void commit_move(Move m);
    void do_move(Move m);       // Actually updates board state
    void do_capture(Move m);    // Updates board and erases piece at m.end
    void undo_move(Move m);

    MOVE_RESULTS check_for_mate() ;
    MOVE_RESULTS check_for_checks() ;

    void setup_start(); // empties board
    void setup_add_piece(char piece, Cord move);
    void setup_rem_piece(Cord move);
    void setup_set_turn(COLOURS col);
    bool setup_finish(); // returns false if setup cannot be finished (invalid pawns, kings, etc)

    MOVE_RESULTS check_pre_move(Move m, bool white_to_move);
    MOVE_RESULTS check_post_move(Move m, bool white_to_move);


    // helpers
    Piece* at(std::string s) const; // returns piece at cord string or nullptr if invalid
    Piece* at(Cord c) const; // returns piece at cord string or nullptr if invalid


    void notify_views();

    void register_view(ChessView* v);

    // friend std::istream& operator>>(std::istream& is, ChessModel& b);
    // friend std::ostream& operator<<(std::ostream& os, const ChessModel& b);
    friend class ComputerPlayer;
    friend class Level1;
    friend class Level2;
    friend class Level3;
    friend class Level4;
};

#endif