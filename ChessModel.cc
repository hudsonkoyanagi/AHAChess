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
  case KING:
    // if moving 2 columnwise, then must be a castle
    if (abs(m.end.row - m.start.row) == 0 && abs(m.end.col - m.start.col) == 2) {
      // check for no initial moves on rook and king

      // check for no check

      // cases both ways
      if (m.end.col == 6) {
        // if the inbetween are not empty, it is an invalid move
        if (!(board[m.start.row][5]->type == EMPTY && board[m.start.row][6]->type == EMPTY)) return INVALID_MOVE;

        std::swap(target->loc, p->loc);
        std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);

        std::swap(board[m.start.row][5]->loc, board[m.start.row][7]->loc);
        std::swap(board[m.start.row][5], board[m.start.row][7]);
      }
      if (m.end.col == 2) {
        // if the inbetween are not empty, it is an invalid move
        if (!(board[m.start.row][1]->type == EMPTY && board[m.start.row][2]->type == EMPTY && board[m.start.row][3]->type == EMPTY)) return INVALID_MOVE;

        std::swap(target->loc, p->loc);
        std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);

        std::swap(board[m.start.row][3]->loc, board[m.start.row][0]->loc);
        std::swap(board[m.start.row][3], board[m.start.row][0]);
      }

      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return SUCCESS;
    }
    // if moving in a direction not 1 or 0, then invalid
    else if (abs(m.end.row - m.start.row) <= 1 && abs(m.end.col - m.start.col) <= 1) {
      // check for if the space is empty
      if (!(target->type == EMPTY)) {
        // can't capture own piece
        if (target->col == p->col) return INVALID_MOVE;

        target->set_empty();

        std::swap(target->loc, p->loc);
        std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);

        history.push_back(m);

        for (auto v : views) {
          v->render(board); // TODO FIX THIS LATER
        }
        // notify_views();
        return CAPTURE;
      }
      else {
        std::swap(target->loc, p->loc); // update piece location
        std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board

        history.push_back(m); // is this necessary? I think it should be, check andrewnotes for what I think about history

        for (auto v : views) {
          v->render(board); // TODO FIX THIS LATER
        }
        // notify_views();
        return SUCCESS;
      }
    }
    else {
      return INVALID_MOVE;
    }
  case KNIGHT:
    // either the row differs by 1 and the col differs by 2, or the row differs by 2 and the col differs by 1
    if (!((abs(m.end.row - m.start.row) == 1 && abs(m.end.col - m.start.col) == 2) || abs(m.end.row - m.start.row) == 2 && abs(m.end.col - m.start.col) == 1)) return INVALID_MOVE;

    // check for if the space is empty
    if (!(target->type == EMPTY)) {
      // can't capture own piece
      if (target->col == p->col) return INVALID_MOVE;

      target->set_empty();

      std::swap(target->loc, p->loc);
      std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);

      history.push_back(m);

      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return CAPTURE;
    }
    else {
      std::swap(target->loc, p->loc); // update piece location
      std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board

      history.push_back(m); // is this necessary? I think it should be, check andrewnotes for what I think about history

      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return SUCCESS;
    }
  case BISHOP:
  {
    // if the change in row is not the same as the change in col, then it is invalid
    if (!(abs(m.end.row - m.start.row) == abs(m.end.col - m.start.col))) return INVALID_MOVE;

    // check if target is same colour
    if (target->col == p->col) return INVALID_MOVE;

    // check if the path to that spot is clear
    int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
    int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
    int curr_row = m.start.row + row_inc;
    int curr_col = m.start.col + col_inc;
    while (curr_row != m.end.row) {
      // if there is anything between them, it is an invalid move
      if (board[curr_row][curr_col]->type != EMPTY) return INVALID_MOVE;
      curr_row += row_inc;
      curr_col += col_inc;
    }

    if (target->type != EMPTY) {
      target->set_empty();

      std::swap(target->loc, p->loc);
      std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
      history.push_back(m);

      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return CAPTURE;
    }
    else {
      std::swap(target->loc, p->loc); // update piece location
      std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board

      history.push_back(m); // is this necessary? I think it should be, check andrewnotes for what I think about history

      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return SUCCESS;
    }
  }
  case ROOK:
  {
    // if both the row and the col don't remain the same it is invalid
    if (!(m.start.row == m.end.row || m.start.col == m.end.col)) return INVALID_MOVE;

    // check if target is same colour
    if (target->col == p->col) return INVALID_MOVE;

    // check if the path to that spot is clear
    if (m.start.row == m.end.row) {
      // if the row stays the same, it moves along the column
      int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
      int curr_col = m.start.col + col_inc;
      while (curr_col != m.end.col) {
        // if there is anything between them, it is an invalid move
        if (board[m.start.row][curr_col]->type != EMPTY) return INVALID_MOVE;
        curr_col += col_inc;
      }
    }
    else {
      // otherwise, it moves along the row
      int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
      int curr_row = m.start.row + row_inc;
      while (curr_row != m.end.row) {
        // if there is anything between them, it is an invalid move
        if (board[curr_row][m.start.col]->type != EMPTY) return INVALID_MOVE;
        curr_row += row_inc;
      }
    }

    if (target->type != EMPTY) {
      target->set_empty();

      std::swap(target->loc, p->loc);
      std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
      history.push_back(m);

      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return CAPTURE;
    }
    else {
      std::swap(target->loc, p->loc); // update piece location
      std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board

      history.push_back(m); // is this necessary? I think it should be, check andrewnotes for what I think about history

      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return SUCCESS;
    }
  }
  case QUEEN:
  {
    // bishop logic
    if (abs(m.end.row - m.start.row) == abs(m.end.col - m.start.col)) {
      // check if target is same colour
      if (target->col == p->col) return INVALID_MOVE;

      // check if the path to that spot is clear
      int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
      int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
      int curr_row = m.start.row + row_inc;
      int curr_col = m.start.col + col_inc;
      while (curr_row != m.end.row) {
        // if there is anything between them, it is an invalid move
        if (board[curr_row][curr_col]->type != EMPTY) return INVALID_MOVE;
        curr_row += row_inc;
        curr_col += col_inc;
      }

      if (target->type != EMPTY) {
        target->set_empty();

        std::swap(target->loc, p->loc);
        std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
        history.push_back(m);

        for (auto v : views) {
          v->render(board); // TODO FIX THIS LATER
        }
        // notify_views();
        return CAPTURE;
      }
      else {
        std::swap(target->loc, p->loc); // update piece location
        std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board

        history.push_back(m); // is this necessary? I think it should be, check andrewnotes for what I think about history

        for (auto v : views) {
          v->render(board); // TODO FIX THIS LATER
        }
        // notify_views();
        return SUCCESS;
      }
    }
    // rook logic
    else if (m.start.row == m.end.row || m.start.col == m.end.col) {
      // check if target is same colour
      if (target->col == p->col) return INVALID_MOVE;

      // check if the path to that spot is clear
      if (m.start.row == m.end.row) {
        // if the row stays the same, it moves along the column
        int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
        int curr_col = m.start.col + col_inc;
        while (curr_col != m.end.col) {
          // if there is anything between them, it is an invalid move
          if (board[m.start.row][curr_col]->type != EMPTY) return INVALID_MOVE;
          curr_col += col_inc;
        }
      }
      else {
        // otherwise, it moves along the row
        int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
        int curr_row = m.start.row + row_inc;
        while (curr_row != m.end.row) {
          // if there is anything between them, it is an invalid move
          if (board[curr_row][m.start.col]->type != EMPTY) return INVALID_MOVE;
          curr_row += row_inc;
        }
      }

      if (target->type != EMPTY) {
        target->set_empty();

        std::swap(target->loc, p->loc);
        std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
        history.push_back(m);

        for (auto v : views) {
          v->render(board); // TODO FIX THIS LATER
        }
        // notify_views();
        return CAPTURE;
      }
      else {
        std::swap(target->loc, p->loc); // update piece location
        std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board

        history.push_back(m); // is this necessary? I think it should be, check andrewnotes for what I think about history

        for (auto v : views) {
          v->render(board); // TODO FIX THIS LATER
        }
        // notify_views();
        return SUCCESS;
      }
    }
    else {
      return INVALID_MOVE;
    }
  }
  default:
    std::cout << "Not implemented yet, ignoring move";
    return INVALID_MOVE;
  }
  return SUCCESS;
}