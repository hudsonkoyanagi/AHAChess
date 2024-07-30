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

    std::array<std::array<Piece*, 8>, 8> boardCopy();

    // helpers
    Piece* at(std::string s) const; // returns piece at cord string or nullptr if invalid
    Piece* at(Cord c) const; // returns piece at cord string or nullptr if invalid


    void notify_views();

    void register_view(ChessView* v);

    void make_move(Cord start,Cord end, std::array<std::array<Piece *, 8>, 8>& b, bool);


    ATTEMPT_RESULT attempt_move(Cord start, Cord end, bool white_to_move,bool isBot = false);
    std::pair<ATTEMPT_RESULT,Move> attempt_move_no_commit(Cord start, Cord end, bool white_to_move);
    std::vector<Cord> get_all_valid_end_cords(Cord start, bool white_to_move);
    std::vector<Cord> get_all_valid_end_cords(Cord start, bool white_to_move,std::array<std::array<Piece *, 8>, 8>& b);
    Cord find_king(std::array<std::array<Piece *, 8>, 8>& b, COLOURS c);
    bool is_white_in_mate();
    bool is_black_in_mate();
    bool is_white_stalemate();
    bool is_black_stalemate();

    bool is_white_in_check(std::array<std::array<Piece *, 8>, 8>& b);
    bool is_black_in_check(std::array<std::array<Piece *, 8>, 8>& b);
    bool is_valid_knight_move(Cord start, Cord end);
    bool is_valid_knight_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b);
    bool is_valid_rook_move(Cord start, Cord end);
    bool is_valid_rook_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b);
    bool is_valid_bishop_move(Cord start, Cord end);
    bool is_valid_bishop_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b);
    bool is_valid_queen_move(Cord start, Cord end);
    bool is_valid_queen_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b);
    bool is_valid_king_move(Cord start, Cord end);
    bool is_valid_king_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b);
    bool is_valid_pawn_move(Cord start, Cord end);
    bool is_valid_pawn_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b);
    



    // friend std::istream& operator>>(std::istream& is, ChessModel& b);
    // friend std::ostream& operator<<(std::ostream& os, const ChessModel& b);
    friend class ComputerPlayer;
    friend class Level1;
    friend class Level2;
    friend class Level3;
    // friend class Level4;
    friend class ChessController;
};  

#endif