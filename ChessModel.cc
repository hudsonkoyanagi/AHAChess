#include "ChessModel.h"
#include "Enums.h"
#include "Cord.h"
#include "Piece.h"
#include "cmath"
#include "utils.h"

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

// finds the king location
Piece* ChessModel::find_king(COLOURS king_col) {
  for (int r = 0;r < 8;++r) {
    for (int c = 0;c < 8;++c) {
      if (board[r][c]->type == KING) {
        if (king_col == WHITE && board[r][c]->col == WHITE) {
          return board[r][c];
        }
        if (king_col == BLACK && board[r][c]->col == BLACK) {
          return board[r][c];
        }
      }
    }
  }
}

// checking if the board has any checks
bool ChessModel::is_in_check(COLOURS king_col) {
  bool white_to_move = (king_col == WHITE) ? false : true;  // if we are checking the WHITE king, check as if it's a BLACK move, and vice versa

  Piece* k = find_king(king_col);

  for (int r = 0;r < 8;++r) {
    for (int c = 0;c < 8;++c) {
      if (board[r][c]->type != EMPTY && board[r][c]->col != king_col) {
        MOVE_RESULTS r = check_pre_move(Move{ board[r][c]->loc, k->loc }, white_to_move);
        if (r == CAPTURE_WITH_CHECK || r == PROMOTE_WITH_CHECK || r == MOVE_WITH_CHECK || r == CASTLE_WITH_CHECK || r == EN_PASSANT_WITH_CHECK) {
          return true;
        }
      }
    }
  }
}

// checks if one side is currently in stalemate
bool ChessModel::is_stalemate_for(COLOURS curr_col) {
  bool white_to_move = (curr_col == WHITE) ? true : false;
  int pawn_dir = (curr_col == WHITE) ? -1 : 1;

  for (int r=0;r<8;++r) {
    for (int c=0;c<8;++c) {
      Piece* p = board[r][c];
      if (p->type != EMPTY && board[r][c]->col == curr_col) {
        switch (p->type) {
          case PAWN:
            if (is_valid(Move{Cord{r, c}, Cord{r + pawn_dir, c}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r + 2*pawn_dir, c}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r + pawn_dir, c - 1}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r + pawn_dir, c + 1}}, white_to_move) != INVALID_MOVE) {
              return false;
            }
            break;
          case KNIGHT:
            if (is_valid(Move{Cord{r, c}, Cord{r-1, c+2}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r-2, c+1}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r-2, c-1}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r-1, c-2}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r+1, c-2}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r+2, c-1}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r+2, c+1}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r+1, c+2}}, white_to_move) != INVALID_MOVE) {
              return false;
            }
            break;
          case BISHOP:
          {
            int cr = r;
            int cc = c;
            // current row & col to top right
            while (cr >= 0 && cc >=0) {
              if (is_valid(Move{Cord{r, c}, Cord{cr, cc}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
              --cr;
              ++cc;
            }

            cr = r;
            cc = c;
            // current row & col to top left
            while (cr >= 0 && cc >=0) {
              if (is_valid(Move{Cord{r, c}, Cord{cr, cc}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
              --cr;
              --cc;
            }

            cr = r;
            cc = c;
            // current row & col to bot left
            while (cr >= 0 && cc >=0) {
              if (is_valid(Move{Cord{r, c}, Cord{cr, cc}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
              ++cr;
              --cc;
            }

            cr = r;
            cc = c;
            // current row & col to bot right
            while (cr >= 0 && cc >=0) {
              if (is_valid(Move{Cord{r, c}, Cord{cr, cc}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
              ++cr;
              ++cc;
            }
            break;
          }
          case ROOK:
          {
            // check left to right
            for (int cc=0;cc<8;cc++) {
              if (is_valid(Move{Cord{r, c}, Cord{r, cc}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
            }
            // check top to bot
            for (int cr=0;cr<8;cr++) {
              if (is_valid(Move{Cord{r, c}, Cord{cr, c}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
            }
            break;
          }
          case QUEEN:
          {
            int cr = r;
            int cc = c;
            // current row & col to top right
            while (cr >= 0 && cc >=0) {
              if (is_valid(Move{Cord{r, c}, Cord{cr, cc}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
              --cr;
              ++cc;
            }

            cr = r;
            cc = c;
            // current row & col to top left
            while (cr >= 0 && cc >=0) {
              if (is_valid(Move{Cord{r, c}, Cord{cr, cc}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
              --cr;
              --cc;
            }

            cr = r;
            cc = c;
            // current row & col to bot left
            while (cr >= 0 && cc >=0) {
              if (is_valid(Move{Cord{r, c}, Cord{cr, cc}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
              ++cr;
              --cc;
            }

            cr = r;
            cc = c;
            // current row & col to bot right
            while (cr >= 0 && cc >=0) {
              if (is_valid(Move{Cord{r, c}, Cord{cr, cc}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
              ++cr;
              ++cc;
            }

            // check left to right
            for (int cc2=0;cc2<8;cc2++) {
              if (is_valid(Move{Cord{r, c}, Cord{r, cc2}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
            }
            // check top to bot
            for (int cr2=0;cr2<8;cr2++) {
              if (is_valid(Move{Cord{r, c}, Cord{cr2, c}}, white_to_move) != INVALID_MOVE) {
                return false;
              }
            }
            
            break;
          }
          case KING:
            if (is_valid(Move{Cord{r, c}, Cord{r, c+1}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r-1, c+1}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r-1, c}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r-1, c-1}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r, c-1}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r+1, c-1}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r+1, c}}, white_to_move) != INVALID_MOVE || is_valid(Move{Cord{r, c}, Cord{r+1, c+1}}, white_to_move) != INVALID_MOVE) {
              return false;
            }
            break;
        }
      }
    }
  }

  return true;
}

// Swaps pieces at start and end
void ChessModel::do_move(Move m) {
  Piece* p = at(m.start);
  Piece* target = at(m.end);
  std::swap(target->loc, p->loc); // update piece location
  std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board

  history.push_back(m);
  p->has_moved = true;
  for (auto v : views) {
    v->render(board); // TODO FIX RENDERING
  }
}

Piece* ChessModel::at(std::string s) const {
  if (!is_valid_cord(s)) return nullptr;
  Cord loc = str_to_cord(s);
  return board[loc.row][loc.col];
}

Piece* ChessModel::at(Cord c) const {
  if(c.col > 7 || c.col <0 || c.row > 7 || c.row <0) {
    std::cout << "FUCKED CORD\n";
    return nullptr;
  }
  return board[c.row][c.col];
}

MOVE_RESULTS ChessModel::make_move(Move m, bool white_to_move) {
  Piece* p = at(m.start);
  Piece* target = at(m.end);

  MOVE_RESULTS result = is_valid(m, white_to_move);
  if (result == INVALID_MOVE) {
    return INVALID_MOVE;
  }

  Move move_to_store{ m.start, m.end };
  move_to_store.moved = p->type;
  move_to_store.taken = target->type;
  move_to_store.move_result = result;
  move_to_store.had_moved_prior = p->has_moved; // allows us to undo move so castling can still be done
  move_to_store.move_result = result;

  commit_move(move_to_store);
  // check_for_checks() and handle
  // check_for_mate() and handle
  // if(m.col is in check) undo_move(); return invalid;
  // if(m.col is in checkmate) undo_move(); return invalid;
  // if(other col is in check) return OTHER_IN_CHECK;
  // if(other col is in mate) return OTHER_IN_MATE;
  std::cout << "Not finished, if you see this it's bad\n";
  return move_to_store.move_result;
}

// Makes the move M, requires the move result from check validity
void ChessModel::commit_move(Move m) {
  Piece* p = at(m.start);
  Piece* target = at(m.end);
  switch (m.move_result) {
  case CASTLE: { // move rook 
    int row = p->col == WHITE ? 8 : 1;
    if (m.start.col > m.start.col) { // Queen side castle
      std::swap(board[m.start.row][3]->loc, board[m.start.row][0]->loc);
      std::swap(board[m.start.row][3], board[m.start.row][0]);
    } else { // King side castle
      std::swap(board[m.start.row][5]->loc, board[m.start.row][7]->loc);
      std::swap(board[m.start.row][5], board[m.start.row][7]);
    }
    do_move(m);
    break;
  }

  case CAPTURE: { // delete target - swap
    target->set_empty(); // only difference from standard move
    do_move(m);
    break;
  }
  case SUCCESS: {// just a move
    do_move(m);
    break;
  }

  case PROMOTION: { // get user input on piece to promote
    // TODO: handle promotion user input
    pawn_to_promote = m.start; // now go get user input come back do promotion and check for checks

    break;
  }
  case EN_PASSANT: {// remove other pawn
    Piece* other_pawn = board[m.start.row][m.end.col];
    other_pawn->set_empty();
    do_move(m);
    break;
  }
  case INVALID_MOVE:
  default:
    std::cerr << "Something terrible has happend in commit move\n";
    return;
  }
}

// Pre: Start and end are both coordinates within the board
// Asserts the validity of a move and returns the result of a move barring check(mate)s without making it
MOVE_RESULTS ChessModel::is_valid(Move m, bool white_to_move) {
  MOVE_RESULTS pre = check_pre_move(m, white_to_move);
  if (pre == INVALID_MOVE) return INVALID_MOVE;
  MOVE_RESULTS post = check_post_move(m,white_to_move);
  std::cout<<"line 205"<<std::endl;
  if(post == SUCCESS) return pre;
  std::cout<<"is_valid running"<<std::endl;
  return post;
}

// Check the piecewise validity of a move without regard for checks
// Only returns SUCCESS, INVALID_MOVE, EN_PASSANT, CASTLE, CAPTURE, SUCCESS
MOVE_RESULTS ChessModel::check_pre_move(Move m, bool white_to_move) {
  if (m.start.row > 7 || m.start.row < 0 || m.end.row > 7 || m.end.row < 0 || m.start.col > 7 || m.start.col < 0 || m.end.col > 7 || m.end.col < 0) return INVALID_MOVE;
  Piece* p = at(m.start);
  Piece* target = at(m.end);
  if ((p->col == WHITE && !white_to_move) || (p->col == BLACK && white_to_move)) return INVALID_MOVE; // correct colour check
  if (m.start.row == m.end.row && m.start.col == m.end.col) return INVALID_MOVE; // piece must move

  switch (p->type) {
  case PAWN: {
    if (m.start.col != m.end.col) { // capture/en pesant
      if (abs(m.start.row - m.end.row) != 1) return INVALID_MOVE; // move one forward
      if (abs(m.start.col - m.end.col) != 1) return INVALID_MOVE; // move one sideways
      if ((p->col == WHITE) && (m.start.row <= m.end.row)) return INVALID_MOVE; // backwards move
      if ((p->col == BLACK) && (m.start.row >= m.end.row)) return INVALID_MOVE; // backwards move
      if (target->col == p->col) return INVALID_MOVE; // same colour capture
      if (target->type == KING) return INVALID_MOVE;  // can't capture king (TODO game should be over here anyway)
      if (target->type != EMPTY) {
        return CAPTURE;
      }

      if (target->type == EMPTY) {

        // both pawns are on the same row but adjacent columns
        Piece* other_pawn = board[m.start.row][m.end.col];

        if (other_pawn->type == PAWN && other_pawn->col != p->col) {
          // last move must be other pawn moving two forward
          Move last_move = history[history.size() - 1];
          if (last_move.moved != PAWN) {

            return INVALID_MOVE;
          }
          if (abs(last_move.start.row - last_move.end.row) != 2) {

            return INVALID_MOVE;
          }
          if (last_move.start.col != last_move.end.col || last_move.start.col != m.end.col) {

            return INVALID_MOVE;
          }
          return EN_PASSANT;
        }
        return INVALID_MOVE;
      }
    }
    if (abs(m.start.row - m.end.row) == 2) { // starting square 
      if ((p->col == WHITE) && (m.start.row != 6) && (m.end.row != 4)) return INVALID_MOVE; // must be on starting square
      if ((p->col == BLACK) && (m.start.row != 1) && (m.end.row != 3)) return INVALID_MOVE; // must be on starting square

      if (target->type != EMPTY) return INVALID_MOVE; // blocked
      if ((p->col == WHITE) && (board[5][m.start.col]->type != EMPTY)) return INVALID_MOVE; // blocked
      if ((p->col == BLACK) && (board[2][m.start.col]->type != EMPTY)) return INVALID_MOVE; // blocked
      return SUCCESS;
    }

    if (abs(m.start.row - m.end.row) == 1) { // forward move, possibly promotion
      if ((p->col == WHITE) && (m.start.row <= m.end.row)) return INVALID_MOVE;
      if ((p->col == BLACK) && (m.start.row >= m.end.row)) return INVALID_MOVE;
      if (target->type != EMPTY) return INVALID_MOVE;
      return SUCCESS;
    }

    return INVALID_MOVE;
  }
  case KING: {
    // if moving 2 columnwise, then must be a castle
    if (abs(m.end.row - m.start.row) == 0 && abs(m.end.col - m.start.col) == 2) {
      if (p->has_moved) return INVALID_MOVE; // king can't have moved

      Piece* should_be_rook;
      int row = 0; // chess notation row not index
      if (p->col == WHITE) {
        row = 1;
        if (m.start.col > m.start.col) should_be_rook = at("a1");
        if (m.start.col < m.start.col) should_be_rook = at("h1");
      } else { // BLACK
        row = 8;
        if (m.start.col > m.start.col) should_be_rook = at("a8");
        if (m.start.col < m.start.col) should_be_rook = at("h8");
      }

      if (should_be_rook->col != p->col) return INVALID_MOVE;
      if (should_be_rook->type != ROOK) return INVALID_MOVE;
      if (should_be_rook->has_moved)    return INVALID_MOVE;

      if (m.start.col > m.start.col) { // Queen side castle
        if (!(at("b" + row)->is_empty()) || !(at("c" + row)->is_empty()) || !(at("d" + row)->is_empty())) return INVALID_MOVE;
      } else { // King side castle
        if (!(at("f" + row)->is_empty()) || !(at("g" + row)->is_empty())) return INVALID_MOVE;
      }
      return CASTLE;
    }

    // if moving in a direction not 1 or 0, then invalid
    else if (abs(m.end.row - m.start.row) <= 1 && abs(m.end.col - m.start.col) <= 1) {
      if (!(target->type == EMPTY)) {
        if (target->col == p->col) return INVALID_MOVE; // can't capture own piece
        return CAPTURE;
      } else {
        return SUCCESS;
      }
    }
    return INVALID_MOVE;
  }
  case KNIGHT: {
    // either the row differs by 1 and the col differs by 2, or the row differs by 2 and the col differs by 1
    if (!((abs(m.end.row - m.start.row) == 1 && abs(m.end.col - m.start.col) == 2) || abs(m.end.row - m.start.row) == 2 && abs(m.end.col - m.start.col) == 1)) return INVALID_MOVE;

    if (target->type == EMPTY) {
      return SUCCESS;
    }
    if (target->col == p->col) return INVALID_MOVE; // can't capture same col piece
    return CAPTURE;
  }
  case BISHOP: {

    // if the change in row is not the same as the change in col, then it is invalid
    if (!(abs(m.end.row - m.start.row) == abs(m.end.col - m.start.col))) return INVALID_MOVE;

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

    if (target->type == EMPTY) {
      return SUCCESS;
    }
    if (target->col == p->col) return INVALID_MOVE; // can't capture same col piece
    return CAPTURE;
  }
  case ROOK: {

    // if both the row and the col don't remain the same it is invalid
    if (!(m.start.row == m.end.row || m.start.col == m.end.col)) return INVALID_MOVE;

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
    } else {
      // otherwise, it moves along the row
      int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
      int curr_row = m.start.row + row_inc;
      while (curr_row != m.end.row) {
        // if there is anything between them, it is an invalid move
        if (board[curr_row][m.start.col]->type != EMPTY) return INVALID_MOVE;
        curr_row += row_inc;
      }
    }

    if (target->type == EMPTY) return SUCCESS;

    if (target->col == p->col) return INVALID_MOVE; // can't capture same col piece
    return CAPTURE;
  }
  case QUEEN: {// TODO seperate out rook/bishop logic?

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
        return CAPTURE;
      } else {
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
      } else {
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
        return CAPTURE;
      } else {
        return SUCCESS;
      }
    } else {
      return INVALID_MOVE;
    }
  }
  default: {
    std::cout << "Not implemented yet, ignoring move";
    return INVALID_MOVE;
  }
  }
  return SUCCESS;
}

// Check the after results of a move (what's in check(mate))
// Pre: check_pre_move has determined piecewise validity
MOVE_RESULTS ChessModel::check_post_move(Move m, bool white_to_move) {
  commit_move(m);

  bool temp_white_in_check = is_in_check(WHITE);
  bool temp_black_in_check = is_in_check(BLACK);

  if (white_to_move) {
    if (temp_white_in_check) {
      undo_move();
      return INVALID_MOVE;
    } else if (temp_black_in_check) {
      undo_move();
      switch (m.move_result) {
      case SUCCESS:
        return MOVE_WITH_CHECK;
      case CAPTURE:
        return CAPTURE_WITH_CHECK;
      case PROMOTION:
        return PROMOTE_WITH_CHECK;
      case EN_PASSANT:
        return EN_PASSANT;
      case CASTLE:
        return CASTLE_WITH_CHECK;
      default:
        return INVALID_MOVE;
      }
    } else {
      return SUCCESS;
    }
  } else { // black to move
    if (temp_black_in_check) {
      undo_move();
      return INVALID_MOVE;
    } else if (temp_white_in_check) {
      undo_move();
      switch (m.move_result) {
      case SUCCESS:
        return MOVE_WITH_CHECK;
      case CAPTURE:
        return CAPTURE_WITH_CHECK;
      case PROMOTION:
        return PROMOTE_WITH_CHECK;
      case EN_PASSANT:
        return EN_PASSANT;
      case CASTLE:
        return CASTLE_WITH_CHECK;
      default:
        return INVALID_MOVE;
      }
    } else {
      return SUCCESS;
    }
  }
}

// MOVE_RESULTS ChessModel::is_valid(Move m, bool white_to_move) {
//   Piece* p = at(m.start);
//   Piece* target = at(m.end);
//   if ((p->col == WHITE && !white_to_move) || (p->col == BLACK && white_to_move)) return INVALID_MOVE; // correct colour check
//   if (m.start.row == m.end.row && m.start.col == m.end.col) return INVALID_MOVE; // piece must move

//   switch (p->type) {
//   case PAWN: {
//     if (m.start.col != m.end.col) { // capture/en pesant
//       if (abs(m.start.row - m.end.row) != 1) return INVALID_MOVE; // move one forward
//       if (abs(m.start.col - m.end.col) != 1) return INVALID_MOVE; // move one sideways
//       if ((p->col == WHITE) && (m.start.row <= m.end.row)) return INVALID_MOVE; // backwards move
//       if ((p->col == BLACK) && (m.start.row >= m.end.row)) return INVALID_MOVE; // backwards move
//       if (target->col == p->col) return INVALID_MOVE; // same colour capture
//       if (target->type == KING) return INVALID_MOVE;  // can't capture king (TODO game should be over here anyway)
//       if (target->type != EMPTY) {
//         return CAPTURE;
//       }

//       if (target->type == EMPTY) {

//         // both pawns are on the same row but adjacent columns
//         Piece* other_pawn = board[m.start.row][m.end.col];

//         if (other_pawn->type == PAWN && other_pawn->col != p->col) {
//           // last move must be other pawn moving two forward
//           Move last_move = history[history.size() - 1];
//           if (last_move.moved != PAWN) {

//             return INVALID_MOVE;
//           }
//           if (abs(last_move.start.row - last_move.end.row) != 2){

//             return INVALID_MOVE;
//           } 
//           if (last_move.start.col != last_move.end.col || last_move.start.col != m.end.col){

//             return INVALID_MOVE;
//           } 
//           return EN_PASSANT;
//         }
//         return INVALID_MOVE;
//       }
//     }
//     if (abs(m.start.row - m.end.row) == 2) { // starting square 
//       if ((p->col == WHITE) && (m.start.row != 6) && (m.end.row != 4)) return INVALID_MOVE; // must be on starting square
//       if ((p->col == BLACK) && (m.start.row != 1) && (m.end.row != 3)) return INVALID_MOVE; // must be on starting square

//       if (target->type != EMPTY) return INVALID_MOVE; // blocked
//       if ((p->col == WHITE) && (board[5][m.start.col]->type != EMPTY)) return INVALID_MOVE; // blocked
//       if ((p->col == BLACK) && (board[2][m.start.col]->type != EMPTY)) return INVALID_MOVE; // blocked
//       return SUCCESS;
//     }

//     if (abs(m.start.row - m.end.row) == 1) { // forward move, possibly promotion
//       if ((p->col == WHITE) && (m.start.row <= m.end.row)) return INVALID_MOVE;
//       if ((p->col == BLACK) && (m.start.row >= m.end.row)) return INVALID_MOVE;
//       if (target->type != EMPTY) return INVALID_MOVE;
//       return SUCCESS;
//     }

//     return INVALID_MOVE;
//   }
//   case KING: {
//     // if moving 2 columnwise, then must be a castle
//     if (abs(m.end.row - m.start.row) == 0 && abs(m.end.col - m.start.col) == 2) {
//       if (p->has_moved) return INVALID_MOVE; // king can't have moved

//       Piece* should_be_rook;
//       int row = 0; // chess notation row not index
//       if (p->col == WHITE) {
//         row = 1;
//         if (m.start.col > m.start.col) should_be_rook = at("a1");
//         if (m.start.col < m.start.col) should_be_rook = at("h1");
//       } else { // BLACK
//         row = 8;
//         if (m.start.col > m.start.col) should_be_rook = at("a8");
//         if (m.start.col < m.start.col) should_be_rook = at("h8");
//       }

//       if (should_be_rook->col != p->col) return INVALID_MOVE;
//       if (should_be_rook->type != ROOK) return INVALID_MOVE;
//       if (should_be_rook->has_moved)    return INVALID_MOVE;

//       if (m.start.col > m.start.col) { // Queen side castle
//         if (!(at("b" + row)->is_empty()) || !(at("c" + row)->is_empty()) || !(at("d" + row)->is_empty())) return INVALID_MOVE;
//       } else { // King side castle
//         if (!(at("f" + row)->is_empty()) || !(at("g" + row)->is_empty())) return INVALID_MOVE;
//       }
//       return CASTLE;
//     }

//     // if moving in a direction not 1 or 0, then invalid
//     else if (abs(m.end.row - m.start.row) <= 1 && abs(m.end.col - m.start.col) <= 1) {
//       if (!(target->type == EMPTY)) {
//         if (target->col == p->col) return INVALID_MOVE; // can't capture own piece
//         return CAPTURE;
//       } else {
//         return SUCCESS;
//       }
//     }
//     return INVALID_MOVE;
//   }
//   case KNIGHT: {
//     // either the row differs by 1 and the col differs by 2, or the row differs by 2 and the col differs by 1
//     if (!((abs(m.end.row - m.start.row) == 1 && abs(m.end.col - m.start.col) == 2) || abs(m.end.row - m.start.row) == 2 && abs(m.end.col - m.start.col) == 1)) return INVALID_MOVE;

//     if (target->type == EMPTY) {
//       return SUCCESS;
//     }
//     if (target->col == p->col) return INVALID_MOVE; // can't capture same col piece
//     return CAPTURE;
//   }
//   case BISHOP: {

//     // if the change in row is not the same as the change in col, then it is invalid
//     if (!(abs(m.end.row - m.start.row) == abs(m.end.col - m.start.col))) return INVALID_MOVE;

//     // check if the path to that spot is clear
//     int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
//     int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
//     int curr_row = m.start.row + row_inc;
//     int curr_col = m.start.col + col_inc;
//     while (curr_row != m.end.row) {
//       // if there is anything between them, it is an invalid move
//       if (board[curr_row][curr_col]->type != EMPTY) return INVALID_MOVE;
//       curr_row += row_inc;
//       curr_col += col_inc;
//     }

//     if (target->type == EMPTY) {
//       return SUCCESS;
//     }
//     if (target->col == p->col) return INVALID_MOVE; // can't capture same col piece
//     return CAPTURE;
//   }
//   case ROOK: {

//     // if both the row and the col don't remain the same it is invalid
//     if (!(m.start.row == m.end.row || m.start.col == m.end.col)) return INVALID_MOVE;

//     // check if the path to that spot is clear
//     if (m.start.row == m.end.row) {
//       // if the row stays the same, it moves along the column
//       int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
//       int curr_col = m.start.col + col_inc;
//       while (curr_col != m.end.col) {
//         // if there is anything between them, it is an invalid move
//         if (board[m.start.row][curr_col]->type != EMPTY) return INVALID_MOVE;
//         curr_col += col_inc;
//       }
//     } else {
//       // otherwise, it moves along the row
//       int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
//       int curr_row = m.start.row + row_inc;
//       while (curr_row != m.end.row) {
//         // if there is anything between them, it is an invalid move
//         if (board[curr_row][m.start.col]->type != EMPTY) return INVALID_MOVE;
//         curr_row += row_inc;
//       }
//     }

//     if (target->type == EMPTY) return SUCCESS;

//     if (target->col == p->col) return INVALID_MOVE; // can't capture same col piece
//     return CAPTURE;
//   }
//   case QUEEN: {// TODO seperate out rook/bishop logic?

//     // bishop logic
//     if (abs(m.end.row - m.start.row) == abs(m.end.col - m.start.col)) {
//       // check if target is same colour
//       if (target->col == p->col) return INVALID_MOVE;

//       // check if the path to that spot is clear
//       int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
//       int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
//       int curr_row = m.start.row + row_inc;
//       int curr_col = m.start.col + col_inc;
//       while (curr_row != m.end.row) {
//         // if there is anything between them, it is an invalid move
//         if (board[curr_row][curr_col]->type != EMPTY) return INVALID_MOVE;
//         curr_row += row_inc;
//         curr_col += col_inc;
//       }

//       if (target->type != EMPTY) {
//         return CAPTURE;
//       } else {
//         return SUCCESS;
//       }
//     }
//     // rook logic
//     else if (m.start.row == m.end.row || m.start.col == m.end.col) {
//       // check if target is same colour
//       if (target->col == p->col) return INVALID_MOVE;

//       // check if the path to that spot is clear
//       if (m.start.row == m.end.row) {
//         // if the row stays the same, it moves along the column
//         int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
//         int curr_col = m.start.col + col_inc;
//         while (curr_col != m.end.col) {
//           // if there is anything between them, it is an invalid move
//           if (board[m.start.row][curr_col]->type != EMPTY) return INVALID_MOVE;
//           curr_col += col_inc;
//         }
//       } else {
//         // otherwise, it moves along the row
//         int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
//         int curr_row = m.start.row + row_inc;
//         while (curr_row != m.end.row) {
//           // if there is anything between them, it is an invalid move
//           if (board[curr_row][m.start.col]->type != EMPTY) return INVALID_MOVE;
//           curr_row += row_inc;
//         }
//       }

//       if (target->type != EMPTY) {
//         return CAPTURE;
//       } else {
//         return SUCCESS;
//       }
//     } else {
//       return INVALID_MOVE;
//     }
//   }
//   default: {
//     std::cout << "Not implemented yet, ignoring move";
//     return INVALID_MOVE;
//   }
//   }
//   return SUCCESS;
// }
