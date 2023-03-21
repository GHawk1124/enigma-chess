#include "board.h"

#include <string>
#include <iostream>

// Check for checks method needs to be debugged before anything
int main() {
  Board board;
  while (true) {
    board.printBoard();
    std::string input = board.getInput();
    unsigned int pos = board.getPos(input.substr(0, 2));
    unsigned int i2 = board.getPos(input.substr(2, 2));
    board.genValidMoves(pos, board.turn);
    board.makeMove(pos, i2);
    board.moves.clear();
  }
  return 0;
}