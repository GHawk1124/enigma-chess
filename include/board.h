#ifndef BOARD_H
#define BOARD_H

#include <tuple>
#include <vector>

/*
 - Pawn = 1
 - Rook = 2
 - Knight = 3
 - Bishop = 4
 - Queen = 5
 - King = 6
 - Empty = 0
*/

class Board {
public:
  Board();
  enum piece {
    whitePawn = 1,
    whiteRook = 2,
    whiteKnight = 3,
    whiteBishop = 4,
    whiteQueen = 5,
    whiteKing = 6,
    Empty = 0,
    blackPawn = 7,
    blackRook = 8,
    blackKnight = 9,
    blackBishop = 10,
    blackQueen = 11,
    blackKing = 12,
  };
  char turn = 'w';
  std::vector<std::tuple<int, int>> moves;
  std::string getInput();
  unsigned int getPos(std::string input);
  void printBoard();
  void makeMove(int pos, int i2);
  void checkPawnMoves(int pos, char turn);
  void checkRookMoves(int pos, char turn);
  void checkBishopMoves(int pos, char turn);
  void checkKnightMoves(int pos, char turn);
  bool checkForChecks(int pos, char turn);
  void checkKingMoves(int pos, char turn);
  void genValidMoves(int pos, char turn);
  void clearBoard();

private:
  unsigned int board[64] = {
      8, 9, 10, 11, 12, 10, 9, 8, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0,
      0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0,  0,  1,  1,  1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 4, 3, 2,
  };
};

#endif