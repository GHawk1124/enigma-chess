#include "board.h"
#include <string>
#include <iostream>

// Random pieces are dissapearing, but other than that genAllValidMoves works well I believe...
// the method for checking if you are in check in the genAllValidMoves method needs to be reworked
// to fix this bug; I think you need to create a temp board maybe and pass that into a checkForChecks method

//
// TO DO:
// En passant
// Stalemate?
// Draw by Repetition

int main() {
  Board board;
  while (true) {
    board.printBoard();
    board.genAllValidMoves(board.turn);
    if (board.moves.size() == 0) {
      break;
    }
    std::string input = board.getInput();
    unsigned int pos = board.getPos(input.substr(0, 2));
    unsigned int i2 = board.getPos(input.substr(2, 2));
    board.printValidMoves();
    board.makeMove(pos, i2);
    board.moves.clear();
  }
  
  int kingPos = board.turn == 'w' ? board.wKingPos : board.bKingPos;
  if (board.checkForChecks(kingPos, board.turn)) {
    std::string turn = board.turn == 'b' ? "white" : "black";
    std::cout << "Checkmate! " << turn << " wins" << std::endl;
  } else {
    std::cout << "Stalemetate!" << std::endl;
  }
  return 0;
}