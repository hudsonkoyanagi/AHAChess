#include <iostream>
#include <string>

#include "ChessController.h"
#include "ChessModel.h"
#include "Player.h"
#include "utils.h"
#include "Level1.h"
#include "Level2.h"
// #include "Level3.h"
// #include "Level4.h"


bool is_valid_computer(const std::string& s) {
  return s.length() == 9 && s.substr(0, 8) == "computer" && '1' <= s[8] &&
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
          p1 = new Level1{ model };
          // p1 = nullptr;
          break;
        case '2':
          p1 = new Level2{ model };
          // p1 = nullptr;
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
          p2 = new Level1{ model };
          break;
        case '2':
          p2 = new Level2{ model };
          // p2 = nullptr;
          break;
        case '3':
          // p2 = new Level3{model};
          p2 = nullptr;
          break;
        case '4':
          // p2 = new Level4{model};
          p2 = nullptr;
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
      MOVE_RESULTS res;
      Move move_made{-1,-1,-1,-1};
      //verify if it is white's turn to move, white is a computer player
      if (white_to_move && p1 != nullptr) {
        std::cout << "Bot moving WHITE" << std::endl;
        res = p1->make_move(white_to_move);
      } else if (!white_to_move && p2 != nullptr) {
        std::cout << "Bot moving BLACK" << std::endl;
        res = p2->make_move(white_to_move);
      } else {
        std::cin >> start >> end;
        if (!is_valid_cord(start) || !is_valid_cord(end)) {
          std::cout << "Invalid chess coordinates, try again\n";
          continue; // retry loop
        }
        move_made = model->make_move(Move{ str_to_cord(start), str_to_cord(end) }, white_to_move);
        res = move_made.move_result;
      }

      if (move_made.check) {
        if (white_to_move) {
          std::cout << "Black is in check.\n";
          white_to_move = !white_to_move;
          break;
        } else {
          std::cout << "White is in check.\n";
          white_to_move = !white_to_move;
          break;

        }
      } else {

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
        case CAPTURE:
          white_to_move = !white_to_move;
          break;
        case SUCCESS:
          white_to_move = !white_to_move;
        default:
          break;
        }
      }
    } else if (command == "resign") {
    } else {
    }
  }
}

void ChessController::setup_loop() { model->setup_start(); }
