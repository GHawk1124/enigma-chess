#include "board.h"
#include <string>
#include <iostream>

// Random pieces are dissapearing, but other than that genAllValidMoves works well I believe...
// the method for checking if you are in check in the genAllValidMoves method needs to be reworked
// to fix this bug; I think you need to create a temp board maybe and pass that into a checkForChecks method





// Checking is not implemented, however I added it to the makeMove method,
// so genValidMoves will still produce moves that are invalid if you are in check,
// makeMove will just reject this move. Not sure if this needs to be changed for the ai.
// The game still won't formally recognize checkmate, however it won't let you make any moves
// Castling should work
// Discovered checks should be handled now
// I added some print statements to help debug (prints valid moves, prints if you are in check)
//
// TO DO:
// En passant
// Checkmate recognition
// Pawn promotion
// Stalemate
// Draw by Repetition

int main() {
  Board board;
  while (true) {
    board.printBoard();
    board.genAllValidMoves(board.turn);
    std::string input = board.getInput();
    unsigned int pos = board.getPos(input.substr(0, 2));
    unsigned int i2 = board.getPos(input.substr(2, 2));
    board.printValidMoves();
    board.makeMove(pos, i2);
    board.moves.clear();
  }
  return 0;
}