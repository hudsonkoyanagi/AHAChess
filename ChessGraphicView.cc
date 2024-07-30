#include "ChessGraphicView.h"
#include <iostream>
#include "utils.h"
#include "Cord.h"
#include "Piece.h"

static const int offset = 50;



ChessGraphicView::ChessGraphicView(int size) : side_length{ size - (2 * offset) }, tile_length{ (size - (2 * offset)) / 8 }, win{ size, size } {
  win.fillRectangle(0,0,size,size, Xwindow::LightGray);
  // std::string s;
  // for (int r=0;r<8;++r) {
  //   for (int c=0;c<8;++c) {
  //     if ((r+c) % 2 == 0) {
  //       win.fillRectangle(tile_length*c, tile_length*r, tile_length, tile_length, Xwindow::White);
  //     }
  //     else {
  //       win.fillRectangle(tile_length*c, tile_length*r, tile_length, tile_length, Xwindow::Green);
  //     }

  //     if (board[r][c] != ' ') {
  //       s = board[r][c];
  //       win.drawString(tile_length*c + tile_length/2, tile_length*r + tile_length/2, s);
  //     }
  //   }
  // }
}

void ChessGraphicView::render() {
  std::string s;
  for (int r = 0;r < 8;++r) {
    for (int c = 0;c < 8;++c) {
      if ((r + c) % 2 == 0) {
        win.fillRectangle(offset + (tile_length * c), offset + (tile_length * r), tile_length, tile_length, Xwindow::White);
      } else {
        win.fillRectangle(offset + (tile_length * c), offset + (tile_length * r), tile_length, tile_length, Xwindow::Purple);
      }
      if (board[r][c] != ' ') {
        s = board[r][c];
        win.drawString(offset + (tile_length * c) + tile_length / 2, offset + (tile_length * r) + tile_length / 2, s);
      }
    }
  }
}

void ChessGraphicView::render(Piece* a, Cord a_cord, Piece* b, Cord b_cord) {
  std::string s;
  board[a_cord.row][a_cord.col] = piece_col_to_char(a->type, a->col);
  board[b_cord.row][b_cord.col] = piece_col_to_char(b->type, b->col);

  if((a_cord.row+a_cord.col)%2 == 0){
    win.fillRectangle(offset + tile_length * a_cord.col, offset + tile_length * a_cord.row, tile_length, tile_length, Xwindow::White);
  } else {
    win.fillRectangle(offset + tile_length * a_cord.col, offset + tile_length * a_cord.row, tile_length, tile_length, Xwindow::Purple);
  }
  if (board[a_cord.row][a_cord.col] != ' ') {
    s = board[a_cord.row][a_cord.col];
    win.drawString(offset + tile_length * a_cord.col + tile_length / 2, offset + tile_length * a_cord.row + tile_length / 2, s);
  }

  if ((b_cord.row + b_cord.col) % 2 == 0) {
    win.fillRectangle(offset + tile_length * b_cord.col, offset + tile_length * b_cord.row, tile_length, tile_length, Xwindow::White);
  } else {
    win.fillRectangle(offset + tile_length * b_cord.col, offset + tile_length * b_cord.row, tile_length, tile_length, Xwindow::Purple);
  }
  if (board[b_cord.row][b_cord.col] != ' ') {
    s = board[b_cord.row][b_cord.col];
    win.drawString(offset + tile_length * b_cord.col + tile_length / 2, offset + tile_length * b_cord.row + tile_length / 2, s);
  }
}

void ChessGraphicView::render(const std::array<std::array<Piece*, 8>, 8>& b) {
  std::string s;
  for(int r = 0; r < 8; r++) {
    for(int c = 0; c < 8; c++) {
      board[r][c] = piece_col_to_char(b[r][c]->type, b[r][c]->col);
      if ((r + c) % 2 == 0) {
        win.fillRectangle(offset + (tile_length * c), offset + (tile_length * r), tile_length, tile_length, Xwindow::White);
      } else {
        win.fillRectangle(offset + (tile_length * c), offset + (tile_length * r), tile_length, tile_length, Xwindow::Purple);
      }
      if (board[r][c] != ' ') {
        s = board[r][c];
        win.drawString(offset + (tile_length * c) + tile_length / 2, offset + (tile_length * r) + tile_length / 2, s);
      }
    }
  }
}