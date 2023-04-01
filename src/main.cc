#include "board.h"

#include <string>
#include <iostream>

// Brainstorming arhitecture for how to do checkmate:
// The engine will effectively need to call genValidMoves on all pieces, so we could
// make a method that does this (e.g. iterate through board and calling the method on all the pieces)
// this would return a vector of all possible moves for a certain side at a certain time.
// Then, if the size of this vector is zero, we know we are in checkmate. However, the downside of this is
// you need to check each possible move to make sure you are not in check once you make that move.
// e.g. moving your bishop creates a discovered check on your king, therefore that move is not valid.
// This is very slow. The way I am optimizing it right now for the playable version is
// that it only checks if you are in check on the specific move you call. For all other valid moves, it won't check.
// So maybe we could implement this with the engine; once it has decided on a move (based on a branch of the
// move tree presumably) it would then check just this branch to see if any of the moves lead to unforseen checks,
// and if they do then it would take that into account (discard that move) and reevaluate.
// We could also potentially just store a boolean in each piece that says whether or not it is in danger of giving
// a discovered check; I'm not sure how this could be implemented, it is just a thought.




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
    std::string input = board.getInput();
    unsigned int pos = board.getPos(input.substr(0, 2));
    unsigned int i2 = board.getPos(input.substr(2, 2));
    board.genValidMoves(pos, board.turn);
    board.makeMove(pos, i2);
    board.moves.clear();
  }
  return 0;
}