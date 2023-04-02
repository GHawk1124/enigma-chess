#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <tuple>
#include <vector>
#include <string>

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
  bool kingsideWRookMoved = false;
  bool queensideWRookMoved = false;
  bool kingsideBRookMoved = false;
  bool queensideBRookMoved = false;
  bool wKingMoved = false;
  bool bKingMoved = false;

  unsigned int getPos(std::string input);
  std::array<unsigned int, 64> decode_fen(const std::string &fen);
  std::array<unsigned int, 64> decode_fen(const char *fen);
  std::string encode_fen(const std::array<unsigned int, 64> &board);
  void printBoard();
  void makeMove(int pos, int i2);

  void checkPawnMoves(int pos, char turn);
  void checkRookMoves(int pos, char turn);
  void checkBishopMoves(int pos, char turn);
  void checkKnightMoves(int pos, char turn);
  bool checkForChecks(int pos, char turn);
  void checkKingMoves(int pos, char turn);
  void genValidMoves(int pos, char turn);
  void genAllValidMoves(char turn);
  void printValidMoves();
  void clearBoard();

private:
  std::array<unsigned int, 64> board = {
      blackRook,   blackKnight, blackBishop, blackQueen,  blackKing,
      blackBishop, blackKnight, blackRook,   blackPawn,   blackPawn,
      blackPawn,   blackPawn,   blackPawn,   blackPawn,   blackPawn,
      blackPawn,   Empty,       Empty,       Empty,       Empty,
      Empty,       Empty,       Empty,       Empty,       Empty,
      Empty,       Empty,       Empty,       Empty,       Empty,
      Empty,       Empty,       Empty,       Empty,       Empty,
      Empty,       Empty,       Empty,       Empty,       Empty,
      Empty,       Empty,       Empty,       Empty,       Empty,
      Empty,       Empty,       Empty,       whitePawn,   whitePawn,
      whitePawn,   whitePawn,   whitePawn,   whitePawn,   whitePawn,
      whitePawn,   whiteRook,   whiteKnight, whiteBishop, whiteQueen,
      whiteKing,   whiteBishop, whiteKnight, whiteRook,
  };
};

#endif