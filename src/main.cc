#include "board.h"

#include <string>
#include <iostream>

// Checking is not implemented, however I added it to the makeMove method, so genValidMoves will still produce moves that are invalid if you are in check, makeMove will just reject this move. Not sure if this needs to be changed for the bot
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