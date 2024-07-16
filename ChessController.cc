#include "ChessController.h"

#include <iostream>
#include <sstream>
#include <string>

void ChessController::input_loop() {
  std::vector<std::string> tokens;
  std::string line;

  bool game_running = false;

  while (std::getline(std::cin, line)) {
    tokens.clear();

    std::istringstream stream(line);
    std::string word;
    while (stream >> word) tokens.push_back(word);


    if(tokens[0] == "game") {

    }
}