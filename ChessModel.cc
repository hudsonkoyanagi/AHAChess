#include "ChessModel.h"
#include "Enums.h"
#include "Cord.h"
#include "Piece.h"
#include "cmath"
#include "utils.h"
#include <cassert>
// Inits to starting board
ChessModel::ChessModel() : pawn_to_promote{ -1, -1 } {
  board[0][0] = new Piece{ Cord{0, 0}, BLACK, ROOK };
  board[0][1] = new Piece{ Cord{0, 1}, BLACK, KNIGHT };
  board[0][2] = new Piece{ Cord{0, 2}, BLACK, BISHOP };
  board[0][3] = new Piece{ Cord{0, 3}, BLACK, QUEEN };
  board[0][4] = new Piece{ Cord{0, 4}, BLACK, KING };
  board[0][5] = new Piece{ Cord{0, 5}, BLACK, BISHOP };
  board[0][6] = new Piece{ Cord{0, 6}, BLACK, KNIGHT };
  board[0][7] = new Piece{ Cord{0, 7}, BLACK, ROOK };

  for (int i = 0; i < 8; i++)
    board[1][i] = new Piece{ Cord{1, i}, BLACK, PAWN };

  for (int i = 2; i <= 5; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = new Piece{Cord{i, j}, COLOURS::NONE, PIECES::EMPTY };
    }
  }

  for (int i = 0; i < 8; i++)
    board[6][i] = new Piece{Cord{6, i}, WHITE, PAWN };

  board[7][0] = new Piece{ Cord{7, 0}, WHITE, ROOK };
  board[7][1] = new Piece{ Cord{7, 1}, WHITE, KNIGHT };
  board[7][2] = new Piece{ Cord{7, 2}, WHITE, BISHOP };
  board[7][3] = new Piece{ Cord{7, 3}, WHITE, QUEEN };
  board[7][4] = new Piece{ Cord{7, 4}, WHITE, KING };
  board[7][5] = new Piece{ Cord{7, 5}, WHITE, BISHOP };
  board[7][6] = new Piece{ Cord{7, 6}, WHITE, KNIGHT };
  board[7][7] = new Piece{ Cord{7, 7}, WHITE, ROOK };
}

ChessModel::~ChessModel() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      delete board[i][j];
    }
  }
  for (auto p : views)
    delete p;
}

void ChessModel::setup_start() {
  
}

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

void ChessModel::empty(){
  for(int i = 0;i<8;i++){
    for(int j =0;j<8;j++){
      board[i][j]->set_empty();
    }
  }
}

// finds the king location
Piece* ChessModel::find_king(COLOURS king_col) {
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
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
  bool white_to_move = (king_col == WHITE) ? false : true; // if we are checking the WHITE king, check as if it's a BLACK move, and vice versa

  Piece* k = find_king(king_col);

  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      if (board[r][c]->type != EMPTY && board[r][c]->col != king_col) {
        MOVE_RESULTS r = check_pre_move(Move{ Cord{r,c}, k->loc }, white_to_move);
        if (r != INVALID_MOVE) return true;
      }
    }
  }
  return false;
}

// checks if one side is currently in stalemate
bool ChessModel::is_stalemate_for(COLOURS curr_col) {
  bool white_to_move = (curr_col == WHITE) ? true : false;
  int pawn_dir = (curr_col == WHITE) ? -1 : 1;

  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      Piece* p = board[r][c];
      if (p->type != EMPTY && board[r][c]->col == curr_col) {
        switch (p->type) {
        case PAWN:
          if (is_valid(Move{ Cord{r, c}, Cord{r + pawn_dir, c} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r + 2 * pawn_dir, c} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r + pawn_dir, c - 1} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r + pawn_dir, c + 1} }, white_to_move).move_result != INVALID_MOVE) {
            return false;
          }
          break;
        case KNIGHT:
          if (is_valid(Move{ Cord{r, c}, Cord{r - 1, c + 2} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r - 2, c + 1} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r - 2, c - 1} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r - 1, c - 2} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r + 1, c - 2} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r + 2, c - 1} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r + 2, c + 1} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r + 1, c + 2} }, white_to_move).move_result != INVALID_MOVE) {
            return false;
          }
          break;
        case BISHOP:
        {
          int cr = r;
          int cc = c;
          // current row & col to top right
          while (cr >= 0 && cc >= 0) {
            if (is_valid(Move{ Cord{r, c}, Cord{cr, cc} }, white_to_move).move_result != INVALID_MOVE) {
              return false;
            }
            --cr;
            ++cc;
          }

          cr = r;
          cc = c;
          // current row & col to top left
          while (cr >= 0 && cc >= 0) {
            if (is_valid(Move{ Cord{r, c}, Cord{cr, cc} }, white_to_move).move_result != INVALID_MOVE) {
              return false;
            }
            --cr;
            --cc;
          }

          cr = r;
          cc = c;
          // current row & col to bot left
          while (cr >= 0 && cc >= 0) {
            if (is_valid(Move{ Cord{r, c}, Cord{cr, cc} }, white_to_move).move_result != INVALID_MOVE) {
              return false;
            }
            ++cr;
            --cc;
          }

          cr = r;
          cc = c;
          // current row & col to bot right
          while (cr >= 0 && cc >= 0) {
            if (is_valid(Move{ Cord{r, c}, Cord{cr, cc} }, white_to_move).move_result != INVALID_MOVE) {
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
          for (int cc = 0; cc < 8; cc++) {
            if (is_valid(Move{ Cord{r, c}, Cord{r, cc} }, white_to_move).move_result != INVALID_MOVE) {
              return false;
            }
          }
          // check top to bot
          for (int cr = 0; cr < 8; cr++) {
            if (is_valid(Move{ Cord{r, c}, Cord{cr, c} }, white_to_move).move_result != INVALID_MOVE) {
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
          while (cr >= 0 && cc >= 0) {
            if (is_valid(Move{ Cord{r, c}, Cord{cr, cc} }, white_to_move).move_result != INVALID_MOVE) {
              return false;
            }
            --cr;
            ++cc;
          }

          cr = r;
          cc = c;
          // current row & col to top left
          while (cr >= 0 && cc >= 0) {
            if (is_valid(Move{ Cord{r, c}, Cord{cr, cc} }, white_to_move).move_result != INVALID_MOVE) {
              return false;
            }
            --cr;
            --cc;
          }

          cr = r;
          cc = c;
          // current row & col to bot left
          while (cr >= 0 && cc >= 0) {
            if (is_valid(Move{ Cord{r, c}, Cord{cr, cc} }, white_to_move).move_result != INVALID_MOVE) {
              return false;
            }
            ++cr;
            --cc;
          }

          cr = r;
          cc = c;
          // current row & col to bot right
          while (cr >= 0 && cc >= 0) {
            if (is_valid(Move{ Cord{r, c}, Cord{cr, cc} }, white_to_move).move_result != INVALID_MOVE) {
              return false;
            }
            ++cr;
            ++cc;
          }

          // check left to right
          for (int cc2 = 0; cc2 < 8; cc2++) {
            if (is_valid(Move{ Cord{r, c}, Cord{r, cc2} }, white_to_move).move_result != INVALID_MOVE) {
              return false;
            }
          }
          // check top to bot
          for (int cr2 = 0; cr2 < 8; cr2++) {
            if (is_valid(Move{ Cord{r, c}, Cord{cr2, c} }, white_to_move).move_result != INVALID_MOVE) {
              return false;
            }
          }

          break;
        }
        case KING:
          if (is_valid(Move{ Cord{r, c}, Cord{r, c + 1} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r - 1, c + 1} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r - 1, c} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r - 1, c - 1} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r, c - 1} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r + 1, c - 1} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r + 1, c} }, white_to_move).move_result != INVALID_MOVE || is_valid(Move{ Cord{r, c}, Cord{r + 1, c + 1} }, white_to_move).move_result != INVALID_MOVE) {
            return false;
          }
          break;
        }
      }
    }
  }

  return true;
}

// checks for checkmate, call it with the colour being checked for checkmate
bool ChessModel::is_checkmate_for(COLOURS curr_col) {
  bool white_to_move = (curr_col == WHITE) ? true : false;
  //if we are currently in check then get out of check
  for (int r = 0;r < 8;++r) {
    for (int c = 0;c < 8;++c) {
      Piece* p = board[r][c];
      if (p->col == curr_col) {
        for (int m=0;m<8;++m) {
          for (int n=0;n<8;++n) {
            Move curr_move = is_valid(Move{Cord{r,c},Cord{m,n} }, white_to_move);
            if (curr_move.move_result != INVALID_MOVE) {
              make_move(curr_move, white_to_move);
              if(is_in_check(p->col)){
                undo_move();
              }
              else {
                return false;
              }
            }
          }
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
  std::swap(target->loc, p->loc);                                          // update piece location
  std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board

  p->has_moved = true;
  for (auto v : views) {
    v->render(board); // TODO FIX RENDERING
  }
}

// undo move
void ChessModel::undo_move() {
  std::cout << history.size() << std::endl;
  assert(history.size() > 0);
  Move m = history.back();
  MOVE_RESULTS result = m.move_result;
  Piece* p = board[m.end.row][m.end.col];
  COLOURS player_colour = p->col;
  COLOURS opponent_colour = (p->col == WHITE) ? BLACK : WHITE;

  switch (result) {
  case SUCCESS:
    std::swap(board[m.start.row][m.start.col]->loc, board[m.end.row][m.end.col]->loc);
    std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
    break;
  case STALEMATE:
    std::swap(board[m.start.row][m.start.col]->loc, board[m.end.row][m.end.col]->loc);
    std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);

    break;
  case BLACK_CHECKMATED:
    std::swap(board[m.start.row][m.start.col]->loc, board[m.end.row][m.end.col]->loc);
    std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
    break;
  case WHITE_CHECKMATED:
    std::swap(board[m.start.row][m.start.col]->loc, board[m.end.row][m.end.col]->loc);
    std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
    break;
  case CAPTURE:
    std::swap(board[m.start.row][m.start.col]->loc, board[m.end.row][m.end.col]->loc);
    std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
    p->type = m.taken;
    p->col = opponent_colour;
    p->has_moved = m.taken_had_moved_prior;
    break;
  case EN_PASSANT:
    std::swap(board[m.start.row][m.start.col]->loc, board[m.end.row][m.end.col]->loc);
    std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
    board[m.start.row][m.end.col]->type = m.taken;
    board[m.start.row][m.end.col]->col = opponent_colour;
    board[m.start.row][m.end.col]->has_moved = m.taken_had_moved_prior;
    break;
  case CASTLE:
    std::swap(board[m.start.row][m.start.col]->loc, board[m.end.row][m.end.col]->loc);
    std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
    // king side castle
    if (m.end.col > m.start.col) {
      // if WHITE, swap rook to bot right
      if (player_colour == WHITE) {
        std::swap(board[7][7]->loc, board[7][5]->loc);
        std::swap(board[7][7], board[7][5]);
        board[7][7]->has_moved = false;
      }
      // otherwise, swap rook to top right
      else {
        std::swap(board[0][7]->loc, board[0][5]->loc);
        std::swap(board[0][7], board[0][5]);
        board[0][7]->has_moved = false;
      }
    }
    // queen side castle
    else {
      // if WHITE, swap rook to bot left
      if (player_colour == WHITE) {
        std::swap(board[7][0]->loc, board[7][3]->loc);
        std::swap(board[7][0], board[7][3]);
        board[7][0]->has_moved = false;
      }
      // otherwise, swap rook to top left
      else {
        std::swap(board[0][0]->loc, board[0][3]->loc);
        std::swap(board[0][0], board[0][3]);
        board[0][0]->has_moved = false;
      }
    }
    break;
  case PROMOTION:
    std::swap(board[m.start.row][m.start.col]->loc, board[m.end.row][m.end.col]->loc);
    std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]);
    board[m.start.row][m.start.col]->type = PAWN;
    break;
  }

  history.pop_back();

  // check if the current is in check; undoing the move will undo the check
  if (m.check) {
    if (player_colour == WHITE) {
      black_in_check = false;
    } else {
      white_in_check = false;
    }
  }
  // check if the previous was in check; undoing the move will redo the check
  else {
    if (history.size() > 0 && history.back().check) {
      if (player_colour == WHITE) {
        white_in_check = true;
      } else {
        black_in_check = true;
      }
    }
  }

  // check if the piece has moved prior; if not, then reset that flag for the piece
  if (m.had_moved_prior == false) {
    board[m.start.row][m.start.col]->has_moved = false;
  }
  std::cout << "undo_move END\n";
}

Piece* ChessModel::at(std::string s) const {
  if (!is_valid_cord(s)) {

    return nullptr;
  }
  Cord loc = str_to_cord(s);
  return board[loc.row][loc.col];
}

Piece* ChessModel::at(Cord c) const {
  if (c.col > 7 || c.col < 0 || c.row > 7 || c.row < 0) {
    return nullptr;
  }
  return board[c.row][c.col];
}

Move ChessModel::make_move(Move m, bool white_to_move) {
  Piece* p = at(m.start);
  Piece* target = at(m.end);

  Move result = is_valid(m, white_to_move);
  if (result.move_result == INVALID_MOVE) {
    return result;
  }

  result.moved = p->type;
  result.taken = target->type;
  result.had_moved_prior = p->has_moved; // allows us to undo move so castling can still be done
  result.taken_had_moved_prior = target->has_moved;

  commit_move(result);


  if (result.check) {
    if(white_to_move) { // black must be in check
      // check_for_mate() and handle
      black_in_check = true;
      white_in_check = false;
    }
    else {
      // check_for_mate() and handle
      black_in_check =false;
      white_in_check = true;
    }
  } else if(is_stalemate_for(BLACK)) {
    result.move_result = STALEMATE;
  } else if(is_stalemate_for(WHITE)) {
    result.move_result = STALEMATE;
  }
  return result;
}

// Makes the move M, requires the move result from check validity
void ChessModel::commit_move(Move m) {
  Piece* p = at(m.start);
  Piece* target = at(m.end);
  m.print();
  switch (m.move_result) {
  case CASTLE:
  { // move rook
    int row = p->col == WHITE ? 8 : 1;
    if (m.start.col > m.start.col) { // Queen side castle
      std::swap(board[m.start.row][3]->loc, board[m.start.row][0]->loc);
      std::swap(board[m.start.row][3], board[m.start.row][0]);
    } else { // King side castle
      std::swap(board[m.start.row][5]->loc, board[m.start.row][7]->loc);
      std::swap(board[m.start.row][5], board[m.start.row][7]);
    }
    do_move(m);
    history.push_back(m);
    break;
  }

  case CAPTURE:
  {                      // delete target - swap
    target->set_empty(); // only difference from standard move
    do_move(m);
    history.push_back(m);
    break;
  }
  case SUCCESS:
  { // just a move
    do_move(m);
    history.push_back(m);
    break;
  }

  case PROMOTION:
  { // get user input on piece to promote
    while (p->type == PAWN) {
      std::cout << "Enter piece to promote: ";
      char pie;
      std::cin >> pie;
      switch (pie) {
      case 'Q':
      case 'q':
        p->type = QUEEN;
        break;
      case 'R':
      case 'r':
        p->type = ROOK;
        break;
      case 'B':
      case 'b':
        p->type = BISHOP;
        break;
      case 'N':
      case 'n':
        p->type = KNIGHT;
        break;
      default:
        std::cout << "\nInvalid piece to promote to. Try again.\n";
      }
    }
    do_move(m);
    history.push_back(m);
    break;
  }
  case EN_PASSANT:
  { // remove other pawn
    Piece* other_pawn = board[m.start.row][m.end.col];
    other_pawn->set_empty();
    do_move(m);
    history.push_back(m);
    break;
  }
  case INVALID_MOVE:
  default:
    std::cerr << "Something terrible has happend in commit move\n";
    return;
  }
}


Move ChessModel::is_valid(Move m, bool white_to_move) {
  MOVE_RESULTS pre = check_pre_move(m, white_to_move);
  m.move_result = pre;
  if (pre == INVALID_MOVE) return m;
  Move post = check_post_move(m, white_to_move);
  return post;
}

// Check the piecewise validity of a move without regard for checks
// Only returns SUCCESS, INVALID_MOVE, EN_PASSANT, CASTLE, CAPTURE, SUCCESS
MOVE_RESULTS ChessModel::check_pre_move(Move m, bool white_to_move) {
  if (m.start.row > 7 || m.start.row < 0 || m.end.row > 7 || m.end.row < 0 || m.start.col > 7 || m.start.col < 0 || m.end.col > 7 || m.end.col < 0)
    return INVALID_MOVE;
  Piece* p = at(m.start);
  Piece* target = at(m.end);
  if ((p->col == WHITE && !white_to_move) || (p->col == BLACK && white_to_move))
    return INVALID_MOVE; // correct colour check
  if (m.start.row == m.end.row && m.start.col == m.end.col)
    return INVALID_MOVE; // piece must move
  switch (p->type) {
  case PAWN:
  {
    if (m.start.col != m.end.col) { // capture/en pesant
      if (abs(m.start.row - m.end.row) != 1)
        return INVALID_MOVE; // move one forward
      if (abs(m.start.col - m.end.col) != 1)
        return INVALID_MOVE; // move one sideways
      if ((p->col == WHITE) && (m.start.row <= m.end.row))
        return INVALID_MOVE; // backwards move
      if ((p->col == BLACK) && (m.start.row >= m.end.row))
        return INVALID_MOVE; // backwards move
      if (target->col == p->col)
        return INVALID_MOVE; // same colour capture
      if (target->type == KING)
        return INVALID_MOVE; // can't capture king (TODO game should be over here anyway)
      if (target->type != EMPTY) {
        return CAPTURE;
      }

      if (target->type == EMPTY) { // en passant
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
      if ((p->col == WHITE) && (m.start.row != 6) && (m.end.row != 4))
        return INVALID_MOVE; // must be on starting square
      if ((p->col == BLACK) && (m.start.row != 1) && (m.end.row != 3))
        return INVALID_MOVE; // must be on starting square

      if (target->type != EMPTY)
        return INVALID_MOVE; // blocked
      if ((p->col == WHITE) && (board[5][m.start.col]->type != EMPTY))
        return INVALID_MOVE; // blocked
      if ((p->col == BLACK) && (board[2][m.start.col]->type != EMPTY))
        return INVALID_MOVE; // blocked

      return SUCCESS;
    }

    if (abs(m.start.row - m.end.row) == 1) { // forward move, possibly promotion
      if ((p->col == WHITE) && (m.start.row <= m.end.row))
        return INVALID_MOVE;
      if ((p->col == BLACK) && (m.start.row >= m.end.row))
        return INVALID_MOVE;
      if (target->type != EMPTY)
        return INVALID_MOVE;
      return SUCCESS;
    }

    return INVALID_MOVE;
  }
  case KING:
  {
    // if moving 2 columnwise, then must be a castle
    if (abs(m.end.row - m.start.row) == 0 && abs(m.end.col - m.start.col) == 2) {
      if (p->has_moved)
        return INVALID_MOVE; // king can't have moved

      Piece* should_be_rook;
      int row = 0; // chess notation row not index
      if (p->col == WHITE) {
        row = 1;
        if (m.start.col > m.end.col)
          should_be_rook = at("a1");
        if (m.start.col < m.end.col)
          should_be_rook = at("h1");
      } else { // BLACK
        row = 8;
        if (m.start.col > m.end.col)
          should_be_rook = at("a8");
        if (m.start.col < m.end.col)
          should_be_rook = at("h8");
      }
      if (should_be_rook == nullptr)
        return INVALID_MOVE;

      if (should_be_rook->col != p->col)
        return INVALID_MOVE;
      if (should_be_rook->type != ROOK)
        return INVALID_MOVE;
      if (should_be_rook->has_moved)
        return INVALID_MOVE;

      if (p->col == WHITE) {
        if (m.start.col > m.start.col) { // Queen side castle
          if (!(at("b1")->is_empty()) || !(at("c1")->is_empty()) || !(at("d1")->is_empty()))
            return INVALID_MOVE;
        } else { // King side castle
          if (!(at("f1")->is_empty()) || !(at("g1")->is_empty()))
            return INVALID_MOVE;
        }
      } else {
        if (m.start.col > m.start.col) { // Queen side castle
          if (!(at("b8")->is_empty()) || !(at("c8")->is_empty()) || !(at("d8")->is_empty()))
            return INVALID_MOVE;
        } else { // King side castle
          if (!(at("f8")->is_empty()) || !(at("g8")->is_empty()))
            return INVALID_MOVE;
        }
      }

      return CASTLE;
    }

    // if moving in a direction not 1 or 0, then invalid
    else if (abs(m.end.row - m.start.row) <= 1 && abs(m.end.col - m.start.col) <= 1) {
      if (!(target->type == EMPTY)) {
        if (target->col == p->col)
          return INVALID_MOVE; // can't capture own piece
        return CAPTURE;
      } else {
        return SUCCESS;
      }
    }
    return INVALID_MOVE;
  }
  case KNIGHT:
  {
    // either the row differs by 1 and the col differs by 2, or the row differs by 2 and the col differs by 1
    if (!((abs(m.end.row - m.start.row) == 1 && abs(m.end.col - m.start.col) == 2) || abs(m.end.row - m.start.row) == 2 && abs(m.end.col - m.start.col) == 1))
      return INVALID_MOVE;

    if (target->type == EMPTY) {
      return SUCCESS;
    }
    if (target->col == p->col)
      return INVALID_MOVE; // can't capture same col piece
    return CAPTURE;
  }
  case BISHOP:
  {

    // if the change in row is not the same as the change in col, then it is invalid
    if (!(abs(m.end.row - m.start.row) == abs(m.end.col - m.start.col)))
      return INVALID_MOVE;

    // check if the path to that spot is clear
    int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
    int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
    int curr_row = m.start.row + row_inc;
    int curr_col = m.start.col + col_inc;
    while (curr_row != m.end.row) {
      // if there is anything between them, it is an invalid move
      if (board[curr_row][curr_col]->type != EMPTY)
        return INVALID_MOVE;
      curr_row += row_inc;
      curr_col += col_inc;
    }

    if (target->type == EMPTY) {
      return SUCCESS;
    }
    if (target->col == p->col)
      return INVALID_MOVE; // can't capture same col piece
    return CAPTURE;
  }
  case ROOK:
  {

    // if both the row and the col don't remain the same it is invalid
    if (!(m.start.row == m.end.row || m.start.col == m.end.col))
      return INVALID_MOVE;

    // check if the path to that spot is clear
    if (m.start.row == m.end.row) {
      // if the row stays the same, it moves along the column
      int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
      int curr_col = m.start.col + col_inc;
      while (curr_col != m.end.col) {
        // if there is anything between them, it is an invalid move
        if (board[m.start.row][curr_col]->type != EMPTY)
          return INVALID_MOVE;
        curr_col += col_inc;
      }
    } else {
      // otherwise, it moves along the row
      int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
      int curr_row = m.start.row + row_inc;
      while (curr_row != m.end.row) {
        // if there is anything between them, it is an invalid move
        if (board[curr_row][m.start.col]->type != EMPTY)
          return INVALID_MOVE;
        curr_row += row_inc;
      }
    }

    if (target->type == EMPTY)
      return SUCCESS;

    if (target->col == p->col)
      return INVALID_MOVE; // can't capture same col piece
    return CAPTURE;
  }
  case QUEEN:
  { // TODO seperate out rook/bishop logic?

    // bishop logic
    if (abs(m.end.row - m.start.row) == abs(m.end.col - m.start.col)) {
      // check if target is same colour
      if (target->col == p->col)
        return INVALID_MOVE;

      // check if the path to that spot is clear
      int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
      int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
      int curr_row = m.start.row + row_inc;
      int curr_col = m.start.col + col_inc;
      while (curr_row != m.end.row) {
        // if there is anything between them, it is an invalid move
        if (board[curr_row][curr_col]->type != EMPTY)
          return INVALID_MOVE;
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
      if (target->col == p->col)
        return INVALID_MOVE;

      // check if the path to that spot is clear
      if (m.start.row == m.end.row) {
        // if the row stays the same, it moves along the column
        int col_inc = (m.end.col - m.start.col) / abs(m.end.col - m.start.col); // +1 or -1, depending on direction of movement
        int curr_col = m.start.col + col_inc;
        while (curr_col != m.end.col) {
          // if there is anything between them, it is an invalid move
          if (board[m.start.row][curr_col]->type != EMPTY)
            return INVALID_MOVE;
          curr_col += col_inc;
        }
      } else {
        // otherwise, it moves along the row
        int row_inc = (m.end.row - m.start.row) / abs(m.end.row - m.start.row); // +1 or -1, depending on direction of movement
        int curr_row = m.start.row + row_inc;
        while (curr_row != m.end.row) {
          // if there is anything between them, it is an invalid move
          if (board[curr_row][m.start.col]->type != EMPTY)
            return INVALID_MOVE;
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
  default:
  {
    return INVALID_MOVE;
  }
  }
  return SUCCESS;
}

// Check the after results of a move (what's in check(mate))
// Pre: check_pre_move has determined piecewise validity
Move ChessModel::check_post_move(Move m, bool white_to_move) {
  commit_move(m);

  Move to_ret = m;

  bool temp_white_in_check = is_in_check(WHITE);
  bool temp_black_in_check = is_in_check(BLACK);

  if (white_to_move) { // m is made by white
    if (temp_white_in_check) {
      to_ret.move_result = INVALID_MOVE;
    } else if (temp_black_in_check) {
      to_ret.check = true;
    }
  } else { // black to move
    if (temp_black_in_check) {
      to_ret.move_result = INVALID_MOVE;
    } else if (temp_white_in_check) {
      to_ret.check = true;
    }
  }
  
  undo_move();
  return to_ret;
}
