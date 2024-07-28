#include <iostream>
#include <string>

#include "ChessController.h"
#include "ChessModel.h"
#include "Player.h"
#include "utils.h"


bool is_valid_computer(const std::string& s) {
  return s.length() == 9 && s.substr(0, 7) == "computer" && '1' <= s[8] &&
         s[8] <= '4';
}

void ChessController::input_loop() {
  std::string command;
  std::string second;
  std::string third;

  std::cout << "Current score: \n";
  std::cout << "White: " << white_wins << std::endl;
  std::cout << "Black: " << black_wins << std::endl;

  while (std::cin >> command) {
    bool p1_good = false;
    bool p2_good = false;

    // TODO implement computes to do this
    if (command == "game") {
      std::cin >> second;  // player 1
      std::cin >> third;   // player 2

      if (second == "human") {
        delete p1;
        p1 = nullptr;
        p1_good = true;
      } else if (is_valid_computer(second)) {
        delete p1;
        p1_good = true;
        switch (second[8]) {
          case '1':
            // p1 = new Level1{model};
            p1 = nullptr;
            break;
          case '2':
            // p1 = new Level2{model};
            p1 = nullptr;
            break;
          case '3':
            // p1 = new Level3{model};
            p1 = nullptr;
            break;
          case '4':
            // p1 = new Level4{model};
            p1 = nullptr;
            break;
          default:
            std::cout << "Computer parsing logic gone wrong: abort\n";
            return;
        }
      } else {
        std::cout << "Invalid player name: '" << second
                  << "'. Please try again.\n";
      }

      if (third == "human") {
        delete p2;
        p2 = nullptr;
        p2_good = true;
      } else if (is_valid_computer(third)) {
        delete p2;
        p2_good = true;
        switch (third[8]) {
          case '1':
            // p2 = new Level1{model};
            p1 = nullptr;
            break;
          case '2':
            // p2 = new Level2{model};
            p1 = nullptr;
            break;
          case '3':
            // p2 = new Level3{model};
            p1 = nullptr;
            break;
          case '4':
            // p2 = new Level4{model};
            p1 = nullptr;
            break;
          default:
            std::cout << "Computer parsing logic gone wrong: abort\n";
            return;
        }
      } else {
        std::cout << "Invalid player name: " << second
                  << ". Please try again.\n";
      }

      if (p1_good && p2_good) game_loop();
    } else if (command == "setup") {
      setup_loop();
      // setup_loop
      std::cout << "Not implemented\n";
      exit(1);
    } else {
      std::cout << "Invalid command: " << command << ". Please try again. \n";
    }
  }
}

void ChessController::game_loop() {
  std::cout << "Game beginning, white to move\n";
  std::string command;
  std::string start, end;
  while (std::cin >> command) {
    if (command == "move") {
      std::cin >> start;
      std::cin >> end;

      if (!is_valid_cord(start) || !is_valid_cord(end)) {
        std::cout << "Invalid chess coordinates, try again\n";
      }

      MOVE_RESULTS res =
          model->make_move(Move{str_to_cord(start), str_to_cord(end)}, white_to_move);
      switch (res) {
        case INVALID_MOVE:
          std::cout << "Invalid move played, try again\n";
          break;
        case BLACK_CHECKMATED:  // end game
          std::cout << "Checkmate! White wins!\n";
          white_wins += 1;
          return;
        case WHITE_CHECKMATED:  // end game
          std::cout << "Checkmate! Black wins!\n";
          black_wins += 1;
          return;
        case STALEMATE:
          std::cout << "Stalemate!\n";
          white_wins += 0.5;
          black_wins += 0.5;
          return;
        case BLACK_IN_CHECK:
          std::cout << "Black is in check.\n";
          white_to_move = !white_to_move;
          break;
        case WHITE_IN_CHECK:
          std::cout << "White is in check.\n";
          white_to_move = !white_to_move;
          break;
        case CAPTURE:
          std::cout << "DEBUG: captured\n";
          white_to_move = !white_to_move;
          break;
        case SUCCESS:
          std::cout << "DEBUG: made move\n";
          white_to_move = !white_to_move;
        default:
          break;
      }

    } else if (command == "resign") {
    } else {
    }
  }
}

void ChessController::setup_loop() { model->setup_start(); }
