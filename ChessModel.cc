#include "ChessModel.h"
#include "Enums.h"
#include "Cord.h"
#include "Piece.h"
#include "cmath"
#include "utils.h"
#include <cassert>
// Inits to starting board
ChessModel::ChessModel() : pawn_to_promote{-1, -1}
{
  board[0][0] = new Piece{Cord{0, 0}, BLACK, ROOK};
  board[0][1] = new Piece{Cord{0, 1}, BLACK, KNIGHT};
  board[0][2] = new Piece{Cord{0, 2}, BLACK, BISHOP};
  board[0][3] = new Piece{Cord{0, 3}, BLACK, QUEEN};
  board[0][4] = new Piece{Cord{0, 4}, BLACK, KING};
  board[0][5] = new Piece{Cord{0, 5}, BLACK, BISHOP};
  board[0][6] = new Piece{Cord{0, 6}, BLACK, KNIGHT};
  board[0][7] = new Piece{Cord{0, 7}, BLACK, ROOK};

  for (int i = 0; i < 8; i++)
    board[1][i] = new Piece{Cord{1, i}, BLACK, PAWN};

  for (int i = 2; i <= 5; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      board[i][j] = new Piece{Cord{i, j}, COLOURS::NONE, PIECES::EMPTY};
    }
  }

  for (int i = 0; i < 8; i++)
    board[6][i] = new Piece{Cord{6, i}, WHITE, PAWN};

  board[7][0] = new Piece{Cord{7, 0}, WHITE, ROOK};
  board[7][1] = new Piece{Cord{7, 1}, WHITE, KNIGHT};
  board[7][2] = new Piece{Cord{7, 2}, WHITE, BISHOP};
  board[7][3] = new Piece{Cord{7, 3}, WHITE, QUEEN};
  board[7][4] = new Piece{Cord{7, 4}, WHITE, KING};
  board[7][5] = new Piece{Cord{7, 5}, WHITE, BISHOP};
  board[7][6] = new Piece{Cord{7, 6}, WHITE, KNIGHT};
  board[7][7] = new Piece{Cord{7, 7}, WHITE, ROOK};
}

ChessModel::~ChessModel()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      delete board[i][j];
    }
  }
  for (auto p : views)
    delete p;
}

std::array<std::array<Piece *, 8>, 8> ChessModel::boardCopy()
{
  std::array<std::array<Piece *, 8>, 8> new_board;
  for (int r = 0; r < 8; ++r)
  {
    for (int c = 0; c < 8; ++c)
    {
      Piece *to_copy = board[r][c];
      new_board[r][c] = new Piece{to_copy->loc, to_copy->col, to_copy->type, to_copy->has_moved};
    }
  }
  return new_board;
}

void ChessModel::notify_views()
{
  for (auto v : views)
  {
    v->render();
  }
}

void ChessModel::register_view(ChessView *v)
{
  v->init_board(board);
  // v->render();
  views.push_back(v);
}

void ChessModel::empty()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      board[i][j]->set_empty();
    }
  }
}

Piece *ChessModel::at(std::string s) const
{
  if (!is_valid_cord(s))
  {

    return nullptr;
  }
  Cord loc = str_to_cord(s);
  return board[loc.row][loc.col];
}

Piece *ChessModel::at(Cord c) const
{
  if (c.col > 7 || c.col < 0 || c.row > 7 || c.row < 0)
  {
    return nullptr;
  }
  return board[c.row][c.col];
}

ATTEMPT_RESULT ChessModel::attempt_move(Cord start, Cord end, bool white_to_move)
{
  Move mv{start,end};
  mv.moved = board[start.row][start.col]->type;
  mv.had_moved_prior = board[start.row][start.col]->has_moved;
  mv.taken_had_moved_prior = board[end.row][end.col]->has_moved;
  mv.taken = board[end.row][end.col]->type;
  char pie;
  bool en_passant = false;
  bool castle = false;
  bool promotion = false;
  Piece *p = at(start);

  if(white_to_move && p->col != WHITE) return FAILURE;
  if(!white_to_move && p->col != BLACK) return FAILURE;

  std::vector<Cord> valid_ends = get_all_valid_end_cords(start, white_to_move);
  bool found = false;
  for (auto c : valid_ends)
  {
    if (c.col == end.col && c.row == end.row) {
      if (p->type == PAWN && white_to_move && end.row == 0) promotion = true;
      if (p->type == PAWN && !white_to_move && end.row == 7) promotion = true;
      if (p->type == PAWN && start.col != end.col && board[end.row][end.col]->type == EMPTY) en_passant = true;
      if (p->type == KING && abs(end.col - start.col) == 2) castle = true;
      found = true;
    }
  }
  if (!found) return FAILURE;
  // Move is semantically legal here

  std::array<std::array<Piece *, 8>, 8> temp_board = boardCopy();

  if(promotion){
    std::cin>>pie;
    switch (pie) {
      case 'Q':
      case 'q':
        temp_board[start.row][start.col]->type = QUEEN;
        break;
      case 'R':
      case 'r':
        temp_board[start.row][start.col]->type = ROOK;
        break;
      case 'B':
      case 'b':
        temp_board[start.row][start.col]->type = BISHOP;
        break;
      case 'N':
      case 'n':
        temp_board[start.row][start.col]->type = KNIGHT;
        break;
      default:
        std::cout << "\nInvalid piece to promote to. Try again.\n";
        return FAILURE;
      }
    Piece *target = temp_board[end.row][end.col];
    if (target->type != EMPTY && target->col != p->col)
      target->set_empty();
    std::swap(temp_board[end.row][end.col], temp_board[start.row][start.col]);
  }
  else if(en_passant){
    Piece* other_pawn = temp_board[start.row][end.col];
    other_pawn->set_empty();
    std::swap(temp_board[end.row][end.col], temp_board[start.row][start.col]);
  }
  else if(castle){
    std::cout << "WAHHHHHHH\n";
    if (start.col > end.col) { // Queen side castle
      std::swap(temp_board[start.row][3], temp_board[start.row][0]);
    } else { // King side castle
      std::swap(temp_board[start.row][5], temp_board[start.row][7]);
    }
    std::swap(temp_board[end.row][end.col], temp_board[start.row][start.col]);
  }
  else{
    Piece *target = temp_board[end.row][end.col];
    if (target->type != EMPTY && target->col != p->col)
      target->set_empty();
    std::swap(temp_board[end.row][end.col], temp_board[start.row][start.col]);
  }
  bool is_white_temp_in_check = is_white_in_check(temp_board);
  bool is_black_temp_in_check = is_black_in_check(temp_board);
  // bool is_in_stalemate = check_for_stalemate(white_to_move);

  ATTEMPT_RESULT ret;

  if (white_to_move && is_white_temp_in_check)
  { // put yourself in check
    ret = FAILURE;
  }
  else if (white_to_move && is_black_temp_in_check)
  {
    // TODO: check for mate
    ret = BLACK_IN_CHECK;
    black_in_check = true;
    mv.check = true;
  }
  else if (!white_to_move && is_black_temp_in_check)
  {// put yourself in check
    ret = FAILURE;
  }
  else if (!white_to_move && is_white_temp_in_check)
  {
    // TODO: check for mate
    ret = WHITE_IN_CHECK;
    white_in_check = true;
    mv.check = true;
  }
  else if (false)
  {
  } // TODO: stalemate
  else
  {
    white_in_check = false;
    black_in_check = false;
    ret = SUCCESS;
  }

  if (ret != FAILURE)
  { // commit the move to real board
    if(promotion){
      mv.move_result = PROMOTION;
      switch (pie) {
        case 'Q':
        case 'q':
          board[start.row][start.col]->type = QUEEN;
          break;
        case 'R':
        case 'r':
          board[start.row][start.col]->type = ROOK;
          break;
        case 'B':
        case 'b':
          board[start.row][start.col]->type = BISHOP;
          break;
        case 'N':
        case 'n':
          board[start.row][start.col]->type = KNIGHT;
          break;
        default:
          std::cout << "\nInvalid piece to promote to. Try again.\n";
          return FAILURE;
        }
      Piece *target = board[end.row][end.col];
      if (target->type != EMPTY && target->col != p->col)
        target->set_empty();
      std::swap(board[end.row][end.col], board[start.row][start.col]);
    }
    else if(en_passant){
      mv.move_result = EN_PASSANT;
      Piece* other_pawn = board[start.row][end.col];
      other_pawn->set_empty();
      std::swap(board[end.row][end.col], board[start.row][start.col]);   
    }
    else if(castle){
      mv.move_result = CASTLE;
      if (start.col > end.col) { // Queen side castle
        std::swap(board[start.row][3], board[start.row][0]);
      } else { // King side castle
        std::swap(board[start.row][5], board[start.row][7]);
      }
      std::swap(board[end.row][end.col], board[start.row][start.col]);
    }
    else{
      mv.move_result = STANDARD;
      Piece *target = board[end.row][end.col];
      if (target->type != EMPTY && target->col != p->col)
        target->set_empty();
      std::swap(board[end.row][end.col], board[start.row][start.col]);
    }
    history.push_back(mv);
  }

  for (auto l : temp_board) for (auto p : l) delete p;
  for(auto v : views) {
    v->render(board);
  }
  return ret;
}

// PRE: white is in check before being called
bool ChessModel::is_white_in_mate(std::array<std::array<Piece *, 8>, 8> &b) {
  std::vector<Cord> moves;
}

bool ChessModel::is_white_in_check(std::array<std::array<Piece *, 8>, 8> &b)
{
  Cord k_loc = find_king(b, WHITE);
  // checking for check
  for (int r = 0; r < 8; ++r)
  {
    for (int c = 0; c < 8; ++c)
    {
      if (b[r][c]->col == BLACK)
      {
        std::vector<Cord> moves = get_all_valid_end_cords(Cord{r, c}, false, b);
        for (auto cord : moves)
        {
          if (cord.row == k_loc.row && cord.col == k_loc.col) // our own king is under attack after our own move (illegal)
          {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool ChessModel::is_black_in_check(std::array<std::array<Piece *, 8>, 8> &b)
{
  Cord k_loc = find_king(b, BLACK);
  // checking for check
  for (int r = 0; r < 8; ++r)
  {
    for (int c = 0; c < 8; ++c)
    {
      if (b[r][c]->col == WHITE)
      {
        std::vector<Cord> moves = get_all_valid_end_cords(Cord{r, c}, true, b);
        for (auto cord : moves)
        {
          if (cord.row == k_loc.row && cord.col == k_loc.col) // our own king is under attack after our own move (illegal)
          {
            return true;
          }
        }
      }
    }
  }
  return false;
}

Cord ChessModel::find_king(std::array<std::array<Piece *, 8>, 8> &b, COLOURS col)
{
  for (int r = 0; r < 8; ++r)
  {
    for (int c = 0; c < 8; ++c)
    {
      if (b[r][c]->type == KING && b[r][c]->col == col)
        return Cord{r, c};
    }
  }
  return Cord { -1, -1 };
}

std::vector<Cord> ChessModel::get_all_valid_end_cords(Cord start, bool white_to_move)
{
  Piece *p = at(start);
  std::vector<Cord> moves;

  if ((white_to_move && p->col == BLACK) || (!white_to_move && p->col == WHITE))
    return moves;

  for (int r = 0; r < 8; ++r)
  {
    for (int c = 0; c < 8; ++c)
    {

      switch (p->type)
      {
      case KNIGHT:
        if (is_valid_knight_move(start, Cord{r, c}))
          moves.push_back(Cord{r, c});
        break;
      case PAWN:
        if (is_valid_pawn_move(start, Cord{r, c}))
          moves.push_back(Cord{r, c});
        break;
      case KING:
        if (is_valid_king_move(start, Cord{r, c}))
          moves.push_back(Cord{r, c});
        break;
      case BISHOP:
        if (is_valid_bishop_move(start, Cord{r, c}))
          moves.push_back(Cord{r, c});
        break;
      case QUEEN:
        if (is_valid_queen_move(start, Cord{r, c}))
          moves.push_back(Cord{r, c});
        break;
      case ROOK:
        if (is_valid_rook_move(start, Cord{r, c}))
          moves.push_back(Cord{r, c});
        break;
      }
    }
  }
  return moves;
}
std::vector<Cord> ChessModel::get_all_valid_end_cords(Cord start, bool white_to_move,std::array<std::array<Piece *, 8>, 8> &b)
{
  Piece *p = b[start.row][start.col];
  std::vector<Cord> moves;

  if ((white_to_move && p->col == BLACK) || (!white_to_move && p->col == WHITE))
    return moves;

  for (int r = 0; r < 8; ++r)
  {
    for (int c = 0; c < 8; ++c)
    {

      switch (p->type)
      {
      case KNIGHT:
        if (is_valid_knight_move(start, Cord{r, c},b))
          moves.push_back(Cord{r, c});
        break;
      case PAWN:
        if (is_valid_pawn_move(start, Cord{r, c},b))
          moves.push_back(Cord{r, c});
        break;
      case KING:
        if (is_valid_king_move(start, Cord{r, c},b))
          moves.push_back(Cord{r, c});
        break;
      case BISHOP:
        if (is_valid_bishop_move(start, Cord{r, c},b))
          moves.push_back(Cord{r, c});
        break;
      case QUEEN:
        if (is_valid_queen_move(start, Cord{r, c},b))
          moves.push_back(Cord{r, c});
        break;
      case ROOK:
        if (is_valid_rook_move(start, Cord{r, c},b))
          moves.push_back(Cord{r, c});
        break;
      }
    }
  }
  return moves;
}

bool ChessModel::is_valid_knight_move(Cord start, Cord end)
{
  Piece *p = at(start);
  Piece *target = at(end);

  // either the row differs by 1 and the col differs by 2, or the row differs by 2 and the col differs by 1
  if (!((abs(end.row - start.row) == 1 && abs(end.col - start.col) == 2) || abs(end.row - start.row) == 2 && abs(end.col - start.col) == 1))
    return false;

  if (target->type == EMPTY)
    return true;

  if (target->col == p->col)
    return false;

  return true;
}
bool ChessModel::is_valid_knight_move(Cord start, Cord end, std::array<std::array<Piece *, 8>, 8>& b)
{
  Piece *p = b[start.row][start.col];
  Piece *target = b[end.row][end.col];

  // either the row differs by 1 and the col differs by 2, or the row differs by 2 and the col differs by 1
  if (!((abs(end.row - start.row) == 1 && abs(end.col - start.col) == 2) || abs(end.row - start.row) == 2 && abs(end.col - start.col) == 1))
    return false;

  if (target->type == EMPTY)
    return true;

  if (target->col == p->col)
    return false;

  return true;
}

bool ChessModel::is_valid_rook_move(Cord start, Cord end)
{
  // if both the row and the col don't remain the same it is invalid
  if (!(start.row == end.row || start.col == end.col))
    return false;

  // check if the path to that spot is clear
  if (start.row == end.row)
  {
    // if the row stays the same, it moves along the column
    int col_inc = (end.col - start.col) / abs(end.col - start.col); // +1 or -1, depending on direction of movement
    int curr_col = start.col + col_inc;
    while (curr_col != end.col && !((curr_col > 7 || curr_col < 0))) {
      // if there is anything between them, it is an invalid move
      if (board[start.row][curr_col]->type != EMPTY) {
        return false;
      }
      curr_col += col_inc;
    }
  }
  else
  {
    // otherwise, it moves along the row
    int row_inc = (end.row - start.row) / abs(end.row - start.row); // +1 or -1, depending on direction of movement
    int curr_row = start.row + row_inc;
    while (curr_row != end.row && !((curr_row > 7 || curr_row < 0)))
    {
      // if there is anything between them, it is an invalid move
      if (board[curr_row][start.col]->type != EMPTY) {
        return false;
      }
      curr_row += row_inc;
    }
  }

  if (board[end.row][end.col]->type == EMPTY)
    return true;

  if (board[end.row][end.col]->col == board[start.row][start.col]->col)
    return false; // can't capture same col piece
  return true;
}
bool ChessModel::is_valid_rook_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b) {
// if both the row and the col don't remain the same it is invalid
  if (!(start.row == end.row || start.col == end.col))
    return false;

  // check if the path to that spot is clear
  if (start.row == end.row)
  {
    // if the row stays the same, it moves along the column
    int col_inc = (end.col - start.col) / abs(end.col - start.col); // +1 or -1, depending on direction of movement
    int curr_col = start.col + col_inc;
    while (curr_col != end.col && !((curr_col > 7 || curr_col < 0))) {
      // if there is anything between them, it is an invalid move
      if (b[start.row][curr_col]->type != EMPTY) {
        return false;
      }
      curr_col += col_inc;
    }
  }
  else
  {
    // otherwise, it moves along the row
    int row_inc = (end.row - start.row) / abs(end.row - start.row); // +1 or -1, depending on direction of movement
    int curr_row = start.row + row_inc;
    while (curr_row != end.row && !((curr_row > 7 || curr_row < 0)))
    {
      // if there is anything between them, it is an invalid move
      if (b[curr_row][start.col]->type != EMPTY) {
        return false;
      }
      curr_row += row_inc;
    }
  }

  if (b[end.row][end.col]->type == EMPTY)
    return true;

  if (b[end.row][end.col]->col == b[start.row][start.col]->col)
    return false; // can't capture same col piece
  return true;
}


bool ChessModel::is_valid_bishop_move(Cord start, Cord end) {
  // if the change in row is not the same as the change in col, then it is invalid
  if (abs(end.row - start.row) != abs(end.col - start.col)) return false;
  // std::cout << start.row << ", " << start.col << std::endl;

  // check if the path to that spot is clear

  int row_inc = end.row > start.row ? 1 : -1;
  int col_inc = end.col > start.col ? 1 : -1;

  int curr_row = start.row + row_inc;
  int curr_col = start.col + col_inc;

  while (curr_row != end.row && !(curr_row > 7 || curr_row <0 || curr_col > 7 || curr_col < 0)) {
    // if there is anything between them, it is an invalid move
    if (board[curr_row][curr_col]->type != EMPTY) {
      return false;
    }
    curr_row += row_inc;
    curr_col += col_inc;
  }

  if (board[end.row][end.col]->type == EMPTY) {
    return true;
  }
  if (board[end.row][end.col]->col == board[start.row][start.col]->col)
    return false; // can't capture same col piece
  return true;
}
bool ChessModel::is_valid_bishop_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b) {
// if the change in row is not the same as the change in col, then it is invalid
    if (!(abs(end.row - start.row) == abs(end.col - start.col)))
      return false;

    // check if the path to that spot is clear
    // int row_inc = (end.row - start.row) / abs(end.row - start.row); // +1 or -1, depending on direction of movement
    // int col_inc = (end.col - start.col) / abs(end.col - start.col); // +1 or -1, depending on direction of movement
    int row_inc = end.row > start.row ? 1 : -1;
    int col_inc = end.col > start.col ? 1 : -1;
    int curr_row = start.row + row_inc;
    int curr_col = start.col + col_inc;
    // if(curr_row > 7 || curr_row <0 || curr_col > 7 || curr_col < 0) return false;
    while (curr_row != end.row && !(curr_row> 7 || curr_row <0 || curr_col > 7 || curr_col < 0)) {
      // if there is anything between them, it is an invalid move
      if (b[curr_row][curr_col]->type != EMPTY) {
        return false;
      }
      curr_row += row_inc;
      curr_col += col_inc;
    }

    if (b[end.row][end.col]->type == EMPTY) {
      return true;
    }
    if (b[end.row][end.col]->col == b[start.row][start.col]->col)
      return false; // can't capture same col piece
    return true;
}

bool ChessModel::is_valid_queen_move(Cord start, Cord end) {
  bool bish =is_valid_bishop_move(start, end);
  bool roo = is_valid_rook_move(start, end);
  // if(bish) std::cout << "naughty bishop\n";
  // if(roo) std::cout << "naughty rook\n";
  return is_valid_bishop_move(start, end) || is_valid_rook_move(start, end);
}
bool ChessModel::is_valid_queen_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b) {
  return is_valid_bishop_move(start, end ,b) || is_valid_rook_move(start, end, b);
}

bool ChessModel::is_valid_king_move(Cord start, Cord end) {
  // if moving 2 columnwise, then must be a castle
  if (abs(end.row - start.row) == 0 && abs(end.col - start.col) == 2) {
    if (board[start.row][start.col]->has_moved)
      return false; // king can't have moved

    Piece* should_be_rook;
    int row = 0; // chess notation row not index
    if (board[start.row][start.col]->col == WHITE) {
      row = 1;
      if (start.col > end.col)
        should_be_rook = at("a1");
      if (start.col < end.col)
        should_be_rook = at("h1");
    } else { // BLACK
      row = 8;
      if (start.col > end.col)
        should_be_rook = at("a8");
      if (start.col < end.col)
        should_be_rook = at("h8");
    }
    if (should_be_rook == nullptr)
      return false;

    if (should_be_rook->col != board[start.row][start.col]->col)
      return false;
    if (should_be_rook->type != ROOK)
      return false;
    if (should_be_rook->has_moved)
      return false;

    if (board[start.row][start.col]->col == WHITE) {
      if (start.col > start.col) { // Queen side castle
        if (!(at("b1")->is_empty()) || !(at("c1")->is_empty()) || !(at("d1")->is_empty()))
          return false;
      } else { // King side castle
        if (!(at("f1")->is_empty()) || !(at("g1")->is_empty()))
          return false;
      }
    } else {
      if (start.col > start.col) { // Queen side castle
        if (!(at("b8")->is_empty()) || !(at("c8")->is_empty()) || !(at("d8")->is_empty()))
          return false;
      } else { // King side castle
        if (!(at("f8")->is_empty()) || !(at("g8")->is_empty()))
          return false;
      }
    }

    return true;
  }

  // if moving in a direction not 1 or 0, then invalid
  else if (abs(end.row - start.row) <= 1 && abs(end.col - start.col) <= 1) {
    if (!(board[end.row][end.col]->type == EMPTY)) {
      if (board[end.row][end.col]->col == board[start.row][start.col]->col)
        return false; // can't capture own piece
      return true;
    } else {
      return true;
    }
  }
  return false;
}
bool ChessModel::is_valid_king_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b) {
  // if moving 2 columnwise, then must be a castle
  if (abs(end.row - start.row) == 0 && abs(end.col - start.col) == 2) {
    if (b[start.row][start.col]->has_moved)
      return false; // king can't have moved

    Piece* should_be_rook;
    int row = 0; // chess notation row not index
    if (b[start.row][start.col]->col == WHITE) {
      row = 1;
      if (start.col > end.col)
        should_be_rook = b[str_to_cord("a1").row][str_to_cord("a1").col];
      if (start.col < end.col)
        should_be_rook = b[str_to_cord("h1").row][str_to_cord("h1").col];
    } else { // BLACK
      row = 8;
      if (start.col > end.col)
        should_be_rook = b[str_to_cord("a8").row][str_to_cord("a8").col];
      if (start.col < end.col)
        should_be_rook = b[str_to_cord("h8").row][str_to_cord("h8").col];
    }
    if (should_be_rook == nullptr)
      return false;

    if (should_be_rook->col != b[start.row][start.col]->col)
      return false;
    if (should_be_rook->type != ROOK)
      return false;
    if (should_be_rook->has_moved)
      return false;

    if (b[start.row][start.col]->col == WHITE) {
      if (start.col > start.col) { // Queen side castle
        if (!(b[str_to_cord("b1").row][str_to_cord("b1").col]->is_empty()) || !(b[str_to_cord("c1").row][str_to_cord("c1").col]->is_empty()) || !(b[str_to_cord("d1").row][str_to_cord("d1").col]->is_empty()))
          return false;
      } else { // King side castle
        if (!(b[str_to_cord("f1").row][str_to_cord("f1").col]->is_empty()) || !(b[str_to_cord("g1").row][str_to_cord("g1").col]->is_empty()))
          return false;
      }
    } else {
      if (start.col > start.col) { // Queen side castle
        if (!(b[str_to_cord("b8").row][str_to_cord("b8").col]->is_empty()) || !(b[str_to_cord("c8").row][str_to_cord("c8").col]->is_empty()) || !(b[str_to_cord("d8").row][str_to_cord("d8").col]->is_empty()))
          return false;
      } else { // King side castle
        if (!(b[str_to_cord("f8").row][str_to_cord("f8").col]->is_empty()) || !(b[str_to_cord("g8").row][str_to_cord("g8").col]->is_empty()))
          return false;
      }
    }

    return true;
  }

  // if moving in a direction not 1 or 0, then invalid
  else if (abs(end.row - start.row) <= 1 && abs(end.col - start.col) <= 1) {
    if (!(b[end.row][end.col]->type == EMPTY)) {
      if (b[end.row][end.col]->col == b[start.row][start.col]->col)
        return false; // can't capture own piece
      return true;
    } else {
      return true;
    }
  }
  return false;
}

bool ChessModel::is_valid_pawn_move(Cord start, Cord end) {
  if (start.col != end.col) { // capture/en pesant
    if (abs(start.row - end.row) != 1)
      return false; // move one forward
    if (abs(start.col - end.col) != 1)
      return false; // move one sideways
    if ((board[start.row][start.col]->col == WHITE) && (start.row <= end.row))
      return false; // backwards move
    if ((board[start.row][start.col]->col == BLACK) && (start.row >= end.row))
      return false; // backwards move
    if (board[end.row][end.col]->col == board[start.row][start.col]->col)
      return false; // same colour capture
    if (board[end.row][end.col]->type == KING)
      return false; // can't capture king (TODO game should be over here anyway)
    if (board[end.row][end.col]->type != EMPTY) {
      return true;
    }

    if (board[end.row][end.col]->type == EMPTY) { // en passant
      // // both pawns are on the same row but adjacent columns
      Piece* other_pawn = board[start.row][end.col];

      if (other_pawn->type == PAWN && other_pawn->col != board[start.row][start.col]->col) {
        // last move must be other pawn moving two forward
        Move last_move = history[history.size() - 1];
        if (last_move.moved != PAWN) {

          return false;
        }
        if (abs(last_move.start.row - last_move.end.row) != 2) {

          return false;
        }
        if (last_move.start.col != last_move.end.col || last_move.start.col != end.col) {

          return false;
        }
        return true;
      }
      return false;
    }
  }
  if (abs(start.row - end.row) == 2) { // starting square
    if ((board[start.row][start.col]->col == WHITE) && (start.row != 6) && (end.row != 4))
      return false; // must be on starting square
    if ((board[start.row][start.col]->col == BLACK) && (start.row != 1) && (end.row != 3))
      return false; // must be on starting square

    if (board[end.row][end.col]->type != EMPTY)
      return false; // blocked
    if ((board[start.row][start.col]->col == WHITE) && (board[5][start.col]->type != EMPTY))
      return false; // blocked
    if ((board[start.row][start.col]->col == BLACK) && (board[2][start.col]->type != EMPTY))
      return false; // blocked

    return true;
  }

  if (abs(start.row - end.row) == 1) { // forward move, possibly promotion
    if ((board[start.row][start.col]->col == WHITE) && (start.row <= end.row))
      return false;
    if ((board[start.row][start.col]->col == BLACK) && (start.row >= end.row))
      return false;
    if (board[end.row][end.col]->type != EMPTY)
      return false;
    return true;
  }

  return false;
}
bool ChessModel::is_valid_pawn_move(Cord start, Cord end, std::array<std::array<Piece*, 8>, 8>& b) {
  if (start.col != end.col) { // capture/en pesant
    if (abs(start.row - end.row) != 1)
      return false; // move one forward
    if (abs(start.col - end.col) != 1)
      return false; // move one sideways
    if ((b[start.row][start.col]->col == WHITE) && (start.row <= end.row))
      return false; // backwards move
    if ((b[start.row][start.col]->col == BLACK) && (start.row >= end.row))
      return false; // backwards move
    if (b[end.row][end.col]->col == board[start.row][start.col]->col)
      return false; // same colour capture
    if (b[end.row][end.col]->type == KING)
      return false; // can't capture king (TODO game should be over here anyway)
    if (b[end.row][end.col]->type != EMPTY) {
      return true;
    }

    if (b[end.row][end.col]->type == EMPTY) { // en passant
      // // both pawns are on the same row but adjacent columns
      Piece* other_pawn = b[start.row][end.col];

      if (other_pawn->type == PAWN && other_pawn->col != b[start.row][start.col]->col) {
        // last move must be other pawn moving two forward
        Move last_move = history[history.size() - 1];
        if (last_move.moved != PAWN) {

          return false;
        }
        if (abs(last_move.start.row - last_move.end.row) != 2) {

          return false;
        }
        if (last_move.start.col != last_move.end.col || last_move.start.col != end.col) {

          return false;
        }
        return true;
      }
      return false;
    }
  }
  if (abs(start.row - end.row) == 2) { // starting square
    if ((b[start.row][start.col]->col == WHITE) && (start.row != 6) && (end.row != 4))
      return false; // must be on starting square
    if ((b[start.row][start.col]->col == BLACK) && (start.row != 1) && (end.row != 3))
      return false; // must be on starting square

    if (b[end.row][end.col]->type != EMPTY)
      return false; // blocked
    if ((b[start.row][start.col]->col == WHITE) && (b[5][start.col]->type != EMPTY))
      return false; // blocked
    if ((b[start.row][start.col]->col == BLACK) && (b[2][start.col]->type != EMPTY))
      return false; // blocked

    return true;
  }

  if (abs(start.row - end.row) == 1) { // forward move, possibly promotion
    if ((b[start.row][start.col]->col == WHITE) && (start.row <= end.row))
      return false;
    if ((b[start.row][start.col]->col == BLACK) && (start.row >= end.row))
      return false;
    if (b[end.row][end.col]->type != EMPTY)
      return false;
    return true;
  }

  return false;
}