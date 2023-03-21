#include "board.h"

#include <string>
#include <iostream>


// Castling needs to be debugged, sometimes you can still castle through check
// King moves need to be debugged I think
// Black pieces get more valid moves than white pieces do (they can move through other pieces for some reason)
// moves vector is cleared in main() because old moves were never removed; should it be cleared at end of makeMove()? or at beginning of genValidMoves()?
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