#include <iostream>
#include <string>

#include "ChessController.h"
#include "ChessModel.h"
#include "Player.h"
#include "utils.h"
#include "Enums.h"
#include "Level1.h"

bool is_valid_computer(const std::string& s) {
  return s.length() == 9 && s.substr(0, 8) == "computer" && '1' <= s[8] &&
    s[8] <= '4';
}

void ChessController::print_board() {
  std::cout << "- +-------------------------------+" << std::endl;

  for (int r = 0; r < 8; r++) {
    std::cout << 8 - r << " " << "|";
    for (int c = 0; c < 8; c++) {
      std::cout << " " << piece_col_to_char(model->board[r][c]->type, model->board[r][c]->col) << " |";
    }
    std::cout << std::endl
      << "- +-------------------------------+" << std::endl;
  }
  std::cout << "  | a | b | c | d | e | f | g | h |" << std::endl
    << std::endl;
}

bool ChessController::isValidBoard() {
  // you must verify that the board contains exactly one white king and exactly one black
  // king; that no pawns are on the first or last row of the board; and that neither king is in check.
  int whiteKingCount = 0;
  int blackKingCount = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (i == 0 || i == 7) {
        if (model->board[i][j]->type == PAWN) {
          return false;
        }
      }
      if (model->board[i][j]->type == KING && model->board[i][j]->col == WHITE) {
        whiteKingCount++;
      }
      if (model->board[i][j]->type == KING && model->board[i][j]->col == BLACK) {
        blackKingCount++;
      }
    }
  }
  if (whiteKingCount != 1 || blackKingCount != 1) {
    // std::cout<<"failing here"<<std::endl;
    return false;
  }
  // if(model->is_in_check(BLACK) || model->is_in_check(WHITE)){
  //   return false;
  // }
  return true;
}


void ChessController::input_loop() {
  std::string command;
  std::string second;
  std::string third;

  std::cout << "Current score: \n";
  std::cout << "White: " << white_wins << std::endl;
  std::cout << "Black: " << black_wins << std::endl;

  bool p1_good = true;
  bool p2_good = true;

  while (std::cin >> command) {
    if (command == "game") {

      std::cin >> second >> third;

      if (is_valid_computer(second)) {
        switch (second[8]) {
        case '1': 
          p1 = new Level1{ model }; 
          p1_good = true; 
          break;
        // case '2': p1 = new Level2{ model }; p1_good = true; break;
        // case '3': p1 = new Level3{ model }; p1_good = true; break;
        // case '4': p1 = new Level4{ model }; p1_good = true; break;
        default: 
          std::cout << "Invalid player: '" << second << "' Please try again.\n";
          p1_good = false;
          p2_good = false;
          delete p1;
          delete p2;
          continue;
        }
      } else if (second == "human") {
        p1 = nullptr;
        p1_good = true;
      } else {
        std::cout << "Invalid player: '" << second << "' Please try again.\n";
        p1_good = false;
        p2_good = false;
        delete p1;
        delete p2;
        continue;
      }

      if (is_valid_computer(third)) {
        switch (third[8]) {
        case '1':
          p2 = new Level1{ model };
          p2_good = true;
          break;
          // case '2': p2 = new Level2{ model }; p2_good = true; break;
          // case '3': p2 = new Level3{ model }; p2_good = true; break;
          // case '4': p2 = new Level4{ model }; p2_good = true; break;
        default:
          std::cout << "Invalid player: '" << third << "' Please try again.\n";
          p1_good = false;
          p2_good = false;
          delete p1;
          delete p2;
          continue;
        }
      } else if (third == "human") {
        p2 = nullptr;
        p2_good = true;
      } else {
        std::cout << "Invalid player: '" << third << "' Please try again.\n";
        p1_good = false;
        p2_good = false;
        delete p1;
        delete p2;
        continue;
      }

      if(p1_good && p2_good) game_loop();
      model->reset();
    } else if (command == "setup") {
      setup_loop();
    } else {
      std::cout << "Invalid command: '" << command << "' Please try again\n";
    }
  }
}

void ChessController::game_loop() {
  std::cout << "Game beginning, white to move\n";
  std::string command;
  std::string start, end;
  while (std::cin >> command) {
    if (command == "move") {
      ATTEMPT_RESULT res;
      bool human_move =false;

      if(p1_is_white && white_to_move) {
        if(p1 == nullptr) {
          std::cin >> start >> end;
          human_move = true;
        } else {
          res = p1->make_move(true);
        }
      } else if (p1_is_white && !white_to_move) {
        if(p2 == nullptr) {
          std::cin >> start >> end;
          human_move = true;
        } else {
          res = p2->make_move(false);
        }
      } else if (!p1_is_white && !white_to_move) {
        if (p1 == nullptr) {
          std::cin >> start >> end;
          human_move = true;
        } else {
          res = p1->make_move(false);
        }
      } else { //(!p1_is_white && white_to_move) 
        if (p2 == nullptr) {
          std::cin >> start >> end;
          human_move = true;
        } else {
          res = p2->make_move(true);
        }
      }
      if(human_move) {
        
      }




      if (!is_valid_cord(start) || !is_valid_cord(end)) {
        std::cout << "Invalid chess coordinates: <" << start << ", " << end << "> Please try again\n";
        continue; // retry loop
      }

      res = model->attempt_move(str_to_cord(start), str_to_cord(end), white_to_move);

      switch (res) {
      case WHITE_CHECKMATED:
        std::cout << "Checkmate! White wins!\n";
        white_wins++;
        return;
      case BLACK_CHECKMATED:
        std::cout << "Checkmate! Black wins!\n";
        black_wins++;
        return;
      case STALEMATE:
        std::cout << "Stalemate!\n";
        black_wins += 0.5;
        white_wins += 0.5;
        return;
      case WHITE_IN_CHECK:
        std::cout << "White is in check.\n";
        white_to_move = !white_to_move;
        break;
      case BLACK_IN_CHECK:
        std::cout << "Black is in check.\n";
        white_to_move = !white_to_move;
        break;
      case SUCCESS:
        white_to_move = !white_to_move;
        break;
      case FAILURE:
      default:
        std::cout << "Invalid move: <" << start << ", " << end << "> Please try again.\n";
        break;
      }
    } else if (command == "resign") {
      if (white_to_move) {
        black_wins++;
      } else {
        white_wins++;
      }
      return;
    } else {
      std::cout << "Invalid game command: '" << command << "' Please try again.\n";
    }
  }
}

void ChessController::setup_loop() {
  model->empty();
  std::cout << "In setup mode" << std::endl;
  std::string command;
  char piece;
  std::string pos;
  std::string colour;
  while (std::cin >> command) {
    if (command == "+") {
      std::cout << "Adding a piece" << std::endl;
      std::cin >> piece;
      std::cin >> pos;
      if (!is_valid_cord(pos)) {
        std::cout << "Invalid coord" << std::endl;
        continue;
      }
      Cord currPos = str_to_cord(pos);
      std::cout << currPos.col << "," << currPos.row << std::endl;
      PIECES pieceType;
      COLOURS pieceColour;
      switch (piece) {
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
    } else if (command == "-") {
      std::cout << "Removing a piece" << std::endl;
      std::cin >> pos;
      if (!is_valid_cord(pos)) {
        std::cout << "invalid coord" << std::endl;
        continue;
      }
      Cord currPos = str_to_cord(pos);
      model->board[currPos.row][currPos.col]->set_empty();
    } else if (command == "=") {
      std::cout << "Changing Colour" << std::endl;
      std::cin >> colour;
      if (colour == "black") {
        white_to_move = false;
      } else if (colour == "white") {
        white_to_move = true;
      } else {
        std::cout << "Invalid input.";
        continue;
      }
    } else if (command == "done") {
      std::cout << "Exiting Setup" << std::endl;
      if (isValidBoard()) {
        input_loop();
      } else {
        std::cout << "Try again, the setup conditions are invalid" << std::endl;
        // model->empty();
        setup_loop();
      }
    } else {
      std::cout << "Invalid Command entered" << std::endl;
      continue;
    }
  }
}
