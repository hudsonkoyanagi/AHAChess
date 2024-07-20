#include "ChessModel.h"
#include "Enums.h"
#include "Cord.h"
#include "Piece.h"
#include "cmath"

// Inits to starting board
ChessModel::ChessModel() : pawn_to_promote{ -1,-1 } {
  board[0][0] = new Piece{ 'r', Cord{0,0}, BLACK, ROOK };
  board[0][1] = new Piece{ 'n', Cord{0,1}, BLACK, KNIGHT };
  board[0][2] = new Piece{ 'b', Cord{0,2}, BLACK, BISHOP };
  board[0][3] = new Piece{ 'q', Cord{0,3}, BLACK, QUEEN };
  board[0][4] = new Piece{ 'k', Cord{0,4}, BLACK, KING };
  board[0][5] = new Piece{ 'b', Cord{0,5}, BLACK, BISHOP };
  board[0][6] = new Piece{ 'n', Cord{0,6}, BLACK, KNIGHT };
  board[0][7] = new Piece{ 'r', Cord{0,7}, BLACK, ROOK };

  for (int i = 0; i < 8; i++)
    board[1][i] = new Piece{ 'p', Cord{1,i}, BLACK, PAWN };

  for (int i = 2; i <= 5; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = new Piece{ ' ', Cord{i,j}, COLOURS::NONE, PIECES::EMPTY };
    }
  }

  for (int i = 0; i < 8; i++)
    board[6][i] = new Piece{ 'P', Cord{6,i}, WHITE, PAWN };

  board[7][0] = new Piece{ 'R', Cord{7, 0}, WHITE, ROOK };
  board[7][1] = new Piece{ 'N', Cord{7, 1}, WHITE, KNIGHT };
  board[7][2] = new Piece{ 'B', Cord{7, 2}, WHITE, BISHOP };
  board[7][3] = new Piece{ 'Q', Cord{7, 3}, WHITE, QUEEN };
  board[7][4] = new Piece{ 'K', Cord{7, 4}, WHITE, KING };
  board[7][5] = new Piece{ 'B', Cord{7, 5}, WHITE, BISHOP };
  board[7][6] = new Piece{ 'N', Cord{7, 6}, WHITE, KNIGHT };
  board[7][7] = new Piece{ 'R', Cord{7, 7}, WHITE, ROOK };
}

ChessModel::~ChessModel() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      delete board[i][j];
    }
  }
  for (auto p : views) delete p;
}


void ChessModel::setup_start() {}

void ChessModel::notify_views() {
  for (auto v : views) {
    v->render();
  }
}

void ChessModel::register_view(ChessView* v) {
  v->init_board(board);
  // v->render();
  views.push_back(v);
}




// Start and end are both coordinates within the board
MOVE_RESULTS ChessModel::make_move(Move m, bool white_to_move) {
  Piece* p = board[m.start.row][m.start.col];
  Piece* target = board[m.end.row][m.end.col];
  if ((p->col == WHITE && !white_to_move) || (p->col == BLACK && white_to_move)) return INVALID_MOVE;
  switch (p->type) {
  case PAWN:
    if (m.start.col != m.end.col) { // capture/en pesant
      if (abs(m.start.row - m.end.row) != 1) return INVALID_MOVE; // move one forward
      if (abs(m.start.col - m.end.col) != 1) return INVALID_MOVE; // move one sideways
      if ((p->col == WHITE) && (m.start.row <= m.end.row)) return INVALID_MOVE; // backwards move
      if ((p->col == BLACK) && (m.start.row >= m.end.row)) return INVALID_MOVE; // backwards move


      if (target->type == EMPTY) return INVALID_MOVE; // no piece TODO: EN PASSANT
      if (target->col == p->col) return INVALID_MOVE; // same colour capture
      if (target->type == KING) return INVALID_MOVE; // can't capture king (TODO game should be over here anyway)

      // TODO: different capture logic? CHECK FOR CHECKS?
      target->set_empty();

      // Should be standard for most moves
      // TODO: can move to other function
      std::swap(target->loc, p->loc);
      std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
      history.push_back(m);
      p->has_moved = true;
      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return CAPTURE;
    }
    if (abs(m.start.row - m.end.row) == 2) { // starting square 
      if ((p->col == WHITE) && (m.start.row != 6) && (m.end.row != 4)) return INVALID_MOVE; // only way to move forward two
      if ((p->col == BLACK) && (m.start.row != 1) && (m.end.row != 3)) return INVALID_MOVE; // only way to move forward two

      if (target->type != EMPTY) return INVALID_MOVE; // blocked
      if ((p->col == WHITE) && (board[5][m.start.col]->type != EMPTY)) return INVALID_MOVE; // blocked
      if ((p->col == BLACK) && (board[2][m.start.col]->type != EMPTY)) return INVALID_MOVE; // blocked

      std::swap(target->loc, p->loc); // update piece location
      std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board
      p->has_moved = true;
      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return SUCCESS;
    }

    if (abs(m.start.row - m.end.row) == 1) { // forward move, possibly promotion
      if ((p->col == WHITE) && (m.start.row <= m.end.row)) return INVALID_MOVE;
      if ((p->col == BLACK) && (m.start.row >= m.end.row)) return INVALID_MOVE;
      if(target->type != EMPTY) return INVALID_MOVE;
     
      // TODO: HANDLE PROMOTION HERE

      std::swap(target->loc, p->loc); // update piece location
      std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board
      p->has_moved = true;
      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return SUCCESS;
    }

    return INVALID_MOVE;
  default:
    std::cout << "Not implemented yet, ignoring move";
    return INVALID_MOVE;
  }
  return SUCCESS;
}