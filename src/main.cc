#include "board.h"
#include <string>
#include <iostream>
#include <vector>
#include <tuple>


// TO DO:
// Draw by Repetition

int main() {
  Board board;
  // board.printBoard();


  std::vector<std::tuple<int, int, int>> validMoves = board.genAllValidMoves(board.turn);
  board.printValidMoves(validMoves);
  validMoves = board.orderMoves(validMoves);
  board.printValidMoves(validMoves);
  
}