#include "ChessController.h"

#include <iostream>
#include <sstream>
#include <string>

bool is_valid_cord(const std::string& str) {
  return ((str.length() == 2) && ('a' <= str[0] && str[0] <= 'h') &&
          ('1' <= str[1] && str[1] <= '8'));
}

// Invalid cord is returned as <-1,-1>
// Returns row, col indexes in board from a chess cord string
Cord str_to_cord(const std::string& str) {
  if (!is_valid_cord(cord)) return Cord{-1,-1};
  int row = 7 - (str[1] - '0' - 1);
  int col = costrrd[0] - 'a';  // converts 'a'-'h' to index
  return Cord{row, col};
}

void ChessController::input_loop() {
  std::vector<std::string> tokens;
  std::string line;

  bool game_running = false;

  while (std::getline(std::cin, line)) {
    tokens.clear();

    std::istringstream stream(line);
    std::string word;
    while (stream >> word) tokens.push_back(word);

    if (tokens[0] == "game") {
      model->reset();
    }
  }

  void ChessController::game_loop() {
    std::string command;
    std::string start, end;
    while (std::cin >> command) {
      if (command == "move") {
        std::cin >> start;
        std::cin >> end;

        if(!is_valid_cord(start) || !is_valid_cord(end)) {
            std::cout << "Invalid chess coordinates, try again\n";
        }

        MOVE_RESULTS res = model->make_move(Move{str_to_cord(start), str_to_cord(end)});
        switch(res) {
          case INVALID_MOVE:
            std::cout << "Invalid move played, try again\n";
            break;
          case BLACK_CHECKMATED:
            std::cout << "Checkmate! White wins!\n";
            break;
          case WHITE_CHECKMATED:
            std::cout << "Checkmate! Black wins!\n";
          case BLACK_IN_CHECK:
            std::cout << "Black is in check.\n";
            break;
          case WHITE_IN_CHECK:
            std::cout << "White is in check.\n";
            break;
          case STALEMATE:
            std::cout "Stalemate!\n";
            break;
          case SUCCESS:
          default:
            break;

        }


      } else if (command == "resign") {
      } else {
      }
    }
  }