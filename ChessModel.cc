#include "ChessModel.h"
#include "Enums.h"
#include "Cord.h"
#include "Piece.h"

// Inits to starting board
ChessModel::ChessModel(ChessTextView* v) : view{v}, pawn_to_promote{-1,-1} {
  board[0][0] = new Piece{'r', Cord{0,0}, BLACK, ROOK};
  board[0][1] = new Piece{'n', Cord{0,1}, BLACK, KNIGHT};
  board[0][2] = new Piece{'b', Cord{0,2}, BLACK, BISHOP};
  board[0][3] = new Piece{'q', Cord{0,3}, BLACK, QUEEN };
  board[0][4] = new Piece{'k', Cord{0,4}, BLACK, KING};
  board[0][5] = new Piece{'b', Cord{0,5}, BLACK, BISHOP};
  board[0][6] = new Piece{'n', Cord{0,6}, BLACK, KNIGHT};
  board[0][7] = new Piece{'r', Cord{0,7}, BLACK, ROOK};

  for (int i = 0; i < 8; i++)
    board[1][i] = new Piece{'p', Cord{1,i}, BLACK, PAWN};

  for (int i = 2; i <= 5; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = new Piece{' ', Cord{i,j}, COLOURS::NONE, PIECES::EMPTY};
    }
  }

  for (int i = 0; i < 8; i++)
    board[6][i] = new Piece{'P', Cord{6,i}, WHITE, PAWN};

  board[7][0] = new Piece{'R', Cord{7, 0}, WHITE, ROOK};
  board[7][1] = new Piece{'N', Cord{7, 1}, WHITE, KNIGHT};
  board[7][2] = new Piece{'B', Cord{7, 2}, WHITE, BISHOP};
  board[7][3] = new Piece{'Q', Cord{7, 3}, WHITE, QUEEN};
  board[7][4] = new Piece{'K', Cord{7, 4}, WHITE, KING};
  board[7][5] = new Piece{'B', Cord{7, 5}, WHITE, BISHOP};
  board[7][6] = new Piece{'N', Cord{7, 6}, WHITE, KNIGHT};
  board[7][7] = new Piece{'R', Cord{7, 7}, WHITE, ROOK};
}

ChessModel::~ChessModel() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      delete board[i][j];
    }
  }
}