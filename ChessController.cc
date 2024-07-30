#include <iostream>
#include <string>

#include "ChessController.h"
#include "ChessModel.h"
// #include "Player.h"
#include "utils.h"
#include "Enums.h"

bool is_valid_computer(const std::string &s)
{
  return s.length() == 9 && s.substr(0, 8) == "computer" && '1' <= s[8] &&
         s[8] <= '4';
}

void ChessController::print_board()
{
  std::cout << "- +-------------------------------+" << std::endl;

  for (int r = 0; r < 8; r++)
  {
    std::cout << 8 - r << " " << "|";
    for (int c = 0; c < 8; c++)
    {
      std::cout << " " << piece_col_to_char(model->board[r][c]->type, model->board[r][c]->col) << " |";
    }
    std::cout << std::endl
              << "- +-------------------------------+" << std::endl;
  }
  std::cout << "  | a | b | c | d | e | f | g | h |" << std::endl
            << std::endl;
}

bool ChessController::isValidBoard()
{
  // you must verify that the board contains exactly one white king and exactly one black
  // king; that no pawns are on the first or last row of the board; and that neither king is in check.
  int whiteKingCount = 0;
  int blackKingCount = 0;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (i == 0 || i == 7)
      {
        if (model->board[i][j]->type == PAWN)
        {
          return false;
        }
      }
      if (model->board[i][j]->type == KING && model->board[i][j]->col == WHITE)
      {
        whiteKingCount++;
      }
      if (model->board[i][j]->type == KING && model->board[i][j]->col == BLACK)
      {
        blackKingCount++;
      }
    }
  }
  if (whiteKingCount != 1 || blackKingCount != 1)
  {
    // std::cout<<"failing here"<<std::endl;
    return false;
  }
  // if(model->is_in_check(BLACK) || model->is_in_check(WHITE)){
  //   return false;
  // }
  return true;
}

void ChessController::input_loop()
{
  std::string command;
  std::string second;
  std::string third;

  std::cout << "Current score: \n";
  std::cout << "White: " << white_wins << std::endl;
  std::cout << "Black: " << black_wins << std::endl;

  bool p1_good = true;
  bool p2_good = true;

  game_loop();
}

void ChessController::game_loop()
{
  std::cout << "Game beginning, white to move\n";
  std::string command;
  std::string start, end;
  while (std::cin >> command)
  {
    if (command == "move")
    {

      std::cin >> start >> end;
      if (!is_valid_cord(start) || !is_valid_cord(end))
      {
        std::cout << "Invalid chess coordinates, try again\n";
        continue; // retry loop
      }

      // MAKE MOVE
      ATTEMPT_RESULT res = model->attempt_move(str_to_cord(start), str_to_cord(end), white_to_move);
      if (res != FAILURE) {
        if(res == WHITE_CHECKMATED) exit(1);
        if(res == BLACK_CHECKMATED) exit(1);
        white_to_move = !white_to_move;

      } else {
        std::cout << "invalid move. try again \n";
      }
    }
  }
}

  void ChessController::setup_loop()
  {
    model->empty();
    std::cout << "In setup mode" << std::endl;
    std::string command;
    char piece;
    std::string pos;
    std::string colour;
    while (std::cin >> command)
    {
      if (command == "+")
      {
        std::cout << "Adding a piece" << std::endl;
        std::cin >> piece;
        std::cin >> pos;
        if (!is_valid_cord(pos))
        {
          std::cout << "invalid coord" << std::endl;
          continue;
        }
        Cord currPos = str_to_cord(pos);
        std::cout << currPos.col << "," << currPos.row << std::endl;
        PIECES pieceType;
        COLOURS pieceColour;
        switch (piece)
        {
        case 'P':
          pieceType = PAWN;
          pieceColour = WHITE;
          break;
        case 'R':
          pieceType = ROOK;
          pieceColour = WHITE;
          break;
        case 'N':
          pieceType = KNIGHT;
          pieceColour = WHITE;
          break;
        case 'B':
          pieceType = BISHOP;
          pieceColour = WHITE;
          break;
        case 'Q':
          pieceType = QUEEN;
          pieceColour = WHITE;
          break;
        case 'K':
          pieceType = KING;
          pieceColour = WHITE;
          break;
        case 'p':
          pieceType = PAWN;
          pieceColour = BLACK;
          break;
        case 'r':
          pieceType = ROOK;
          pieceColour = BLACK;
          break;
        case 'n':
          pieceType = KNIGHT;
          pieceColour = BLACK;
          break;
        case 'b':
          pieceType = BISHOP;
          pieceColour = BLACK;
          break;
        case 'q':
          pieceType = QUEEN;
          pieceColour = BLACK;
          break;
        case 'k':
          pieceType = KING;
          pieceColour = BLACK;
          break;
        default:
          std::cout << "Invalid piece entered" << std::endl;
          continue;
        }
        model->board[currPos.row][currPos.col]->type = pieceType;
        model->board[currPos.row][currPos.col]->col = pieceColour;
        model->board[currPos.row][currPos.col]->loc = currPos;
        print_board();
      }
      else if (command == "-")
      {
        std::cout << "Removing a piece" << std::endl;
        std::cin >> pos;
        if (!is_valid_cord(pos))
        {
          std::cout << "invalid coord" << std::endl;
          continue;
        }
        Cord currPos = str_to_cord(pos);
        model->board[currPos.row][currPos.col]->set_empty();
      }
      else if (command == "=")
      {
        std::cout << "Changing Colour" << std::endl;
        std::cin >> colour;
        if (colour == "black")
        {
          white_to_move = false;
        }
        else if (colour == "white")
        {
          white_to_move = true;
        }
        else
        {
          std::cout << "Invalid input.";
          continue;
        }
      }
      else if (command == "done")
      {
        std::cout << "Exiting Setup" << std::endl;
        if (isValidBoard())
        {
          input_loop();
        }
        else
        {
          std::cout << "Try again, the setup conditions are invalid" << std::endl;
          // model->empty();
          setup_loop();
        }
      }
      else
      {
        std::cout << "Invalid Command entered" << std::endl;
        continue;
      }
    }
  }
