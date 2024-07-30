#include <iostream>
#include <string>

#include "ChessController.h"
#include "ChessModel.h"
#include "Player.h"
#include "utils.h"
#include "Level1.h"
#include "Level2.h"
#include "Enums.h"
// #include "Level3.h"
// #include "Level4.h"


bool is_valid_computer(const std::string& s) {
  return s.length() == 9 && s.substr(0, 8) == "computer" && '1' <= s[8] &&
    s[8] <= '4';
}

void ChessController::print_board(){
    std::cout << "- +-------------------------------+" << std::endl;

  for (int r = 0; r < 8; r++) {
    std::cout << 8 - r << " " << "|";
    for (int c = 0; c < 8; c++) {
      std::cout << " " << piece_col_to_char(model->board[r][c]->type,model->board[r][c]->col) << " |";
    }
    std::cout << std::endl << "- +-------------------------------+" << std::endl;
  }
  std::cout << "  | a | b | c | d | e | f | g | h |" << std::endl << std::endl;
}

bool ChessController::isValidBoard(){
// you must verify that the board contains exactly one white king and exactly one black
// king; that no pawns are on the first or last row of the board; and that neither king is in check. 
int whiteKingCount = 0;
int blackKingCount = 0;
  for(int i = 0;i<8;i++){
    for(int j = 0;j<8;j++){
      if(i == 0 || i == 7){
        if(model->board[i][j]->type == PAWN){
          return false;
        }
      }
      if(model->board[i][j]->type == KING && model->board[i][j]->col == WHITE){
        whiteKingCount++;
      }
      if(model->board[i][j]->type == KING && model->board[i][j]->col == BLACK){
        blackKingCount++;
      }
    }
  }
  if(whiteKingCount!= 1 || blackKingCount != 1){
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
      // if(isValidBoard()){
      //   input_loop();
      // }
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

void ChessController::setup_loop() { 
  model->empty();
  std::cout<<"In setup mode"<<std::endl;
  std::string command;
  char piece; 
  std::string pos;
  std::string colour;
  while(std::cin>>command){
    if(command == "+"){
      std::cout<<"Adding a piece"<<std::endl;
      std::cin>>piece;
      std::cin>>pos;
      if(!is_valid_cord(pos)){
        std::cout<<"invalid coord"<<std::endl;
        continue;
      }
      Cord currPos = str_to_cord(pos);
      std::cout<<currPos.col<<","<<currPos.row<<std::endl;
      PIECES pieceType;
      COLOURS pieceColour;
      switch (piece) {
        case 'P': pieceType = PAWN; pieceColour = WHITE; break;
        case 'R': pieceType = ROOK; pieceColour = WHITE; break;
        case 'N': pieceType = KNIGHT; pieceColour = WHITE; break;
        case 'B': pieceType = BISHOP; pieceColour = WHITE; break;
        case 'Q': pieceType = QUEEN; pieceColour = WHITE; break;
        case 'K': pieceType = KING; pieceColour = WHITE; break;
        case 'p': pieceType = PAWN; pieceColour = BLACK; break;
        case 'r': pieceType = ROOK; pieceColour = BLACK; break;
        case 'n': pieceType = KNIGHT; pieceColour = BLACK; break;
        case 'b': pieceType = BISHOP; pieceColour = BLACK; break;
        case 'q': pieceType = QUEEN; pieceColour = BLACK; break;
        case 'k': pieceType = KING; pieceColour = BLACK; break;
        default:
          std::cout << "Invalid piece entered" << std::endl;
          continue;
      }
      model->board[currPos.row][currPos.col]->type = pieceType;
      model->board[currPos.row][currPos.col]->col = pieceColour;
      model->board[currPos.row][currPos.col]->loc = currPos;
      print_board();
    }
    else if(command == "-"){
      std::cout<<"Removing a piece"<<std::endl;
      std::cin>>pos;
      if(!is_valid_cord(pos)){
        std::cout<<"invalid coord"<<std::endl;
        continue;
      }
      Cord currPos = str_to_cord(pos);
      model->board[currPos.row][currPos.col]->set_empty();
    }
    else if(command == "="){
      std::cout<<"Changing Colour"<<std::endl;
      std::cin>>colour;
      if(colour == "black"){
        white_to_move = false;
      }
      else if(colour == "white"){
        white_to_move = true;
      }
      else{
        std::cout << "Invalid input.";
        continue;
      }
    }
    else if(command == "done"){
      std::cout<<"Exiting Setup"<<std::endl;
      if(isValidBoard()){
        input_loop();
      }
      else{
        std::cout<<"Try again, the setup conditions are invalid"<<std::endl;
        // model->empty();
        setup_loop();
      }
    }
    else{
      std::cout<<"Invalid Command entered"<<std::endl;
      continue;
    }
  }
 }
