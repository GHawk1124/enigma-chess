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
  std::vector<std::tuple<int, int, int>> moves;
  std::string getInput();
  bool white_kingside_castle = true;
  bool white_queenside_castle = true;
  bool black_kingside_castle = true;
  bool black_queenside_castle = true;
  bool kingsideWRookMoved = false;
  bool queensideWRookMoved = false;
  bool kingsideBRookMoved = false;
  bool queensideBRookMoved = false;
  bool wKingMoved = false;
  bool bKingMoved = false;
  int wKingPos = -1;
  int bKingPos = -1;
  std::tuple<bool, int> enPassant = std::make_tuple(false, -1);
  std::tuple<bool, bool> checkmateDrawInformation =
      std::make_tuple(false, false);
  int movesSinceLastPawnMovedAndPieceTaken = 0;
  // std::unordered_map<std::string, int> boardConfigurations;

  unsigned int getPos(std::string input);
  std::array<unsigned int, 64> decode_fen(const std::string &fen);
  std::array<unsigned int, 64> decode_fen_c(const char *fen);
  std::array<unsigned int, 64> getBoard();
  void setBoard(std::array<unsigned int, 64> board);
  std::string encode_fen(const std::array<unsigned int, 64> &board);
  char* encode_fen_c(const std::array<unsigned int, 64> &board);
  void printBoard();
  void makeMove(std::tuple<int, int, int> move);
  void unmakeMove(std::tuple<int, int, int> move, int i2BeforeMove, int pieceMoved);
  std::tuple<bool, int> enPassantMove = std::make_tuple(false, -1);

  void checkPawnMoves(int pos, char turn);
  void checkRookMoves(int pos, char turn);
  void checkBishopMoves(int pos, char turn);
  void checkKnightMoves(int pos, char turn);
  bool checkForChecks(int pos, char turn);
  void checkKingMoves(int pos, char turn);
  void genValidMoves(int pos, char turn);
  std::vector<std::tuple<int, int, int>> genAllValidMoves(char turn);
  void printValidMoves(std::vector<std::tuple<int, int, int>> inputMovesVector);
  void clearBoard();

  double evaluate();
  std::tuple<int, int, int> runMiniMax(int depth);
  std::tuple<int, int, int> miniMax(int depth, std::vector<std::tuple<int, int, int>> possibleMoves);
  double miniMaxRec(int depth, double alpha, double beta);
  std::vector<std::tuple<int, int, int>> orderMoves(std::vector<std::tuple<int, int, int>> moves);

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