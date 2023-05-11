#include "board.h"

#include <array>
#include <unordered_map>
#include <vector>
#include <set>

int Board::evaluate() {
  // std::unordered_map<int, double> wPawnSquareToBonus = {{}}
  std::set diagonalSquares = {0, 7, 9, 14, 18, 21, 27, 28, 35, 36, 42, 45, 49, 54, 56, 63};
  std::set centerSquares = {27, 28, 35, 36};
  int turnSwap = turn == 'w'? 1 : -1;
  double score = 0;
  for (int i = 0; i < 64; i++) {
    switch (this->board[i]) {
      case whitePawn:
        if ((i / 8) == 4) {
          score += 1.5;
        } else if ((i / 8) == 5) {
          score += 1.3;
        } else {
          score += 1;
        }
        if (centerSquares.count(i) == 1) {
          score += 0.4;
        }
        break;
      case whiteBishop:
        if (diagonalSquares.count(i) == 1) {
          score += 3.5;
        } else {
          score += 3;
        }
        break;
      case whiteKnight:
        score += 3;
        break;
      case whiteRook:
        score += 5;
        break;
      case whiteQueen:
        score += 11;
        break;
      case blackPawn:
        if ((i / 8) == 3) {
          score -= 1.5;
        } else if ((i / 8) == 2) {
          score -= 1.3;
        } else {
          score -= 1;
        }
        if (centerSquares.count(i) == 1) {
          score -= 0.4;
        }
        break;
      case blackBishop:
        if (diagonalSquares.count(i) == 1) {
          score -= 3.5;
        } else {
          score -= 3;
        }
        break;
      case blackKnight:
        score -= 3;
        break;
      case blackRook:
        score -= 5;
        break;
      case blackQueen:
        score -= 11;
        break;
      default:
        break;
    }
  }
  return turnSwap * score;
}


// Iterate through the first depth (all of black's possible moves) and call the recursive function on each of these moves
// This will give a score to all of these moves which you can then take the best score and therefore best move
// In this way you can keep track of which move to make (a number by itself is meaningless) I don't know a better way to do this
std::tuple<int, int> Board::miniMax(int depth) {
  std::vector<std::tuple<int, int>> possibleMoves = this->genAllValidMoves(this->turn);
  int bestMoveScore = INT_MIN;
  std::tuple<int, int> bestMove = possibleMoves[0];

  for (std::tuple<int, int> move : possibleMoves) {
    int pieceMoved = this->board[std::get<0>(move)];
    int pieceAtI2 = this->board[std::get<1>(move)];
    this->makeMove(move);
    int moveScore = this->miniMaxRec(depth, this->evaluate());
    if (moveScore > bestMoveScore) {
      bestMoveScore = moveScore;
      bestMove = move;
    }
    this->unmakeMove(move, pieceAtI2, pieceMoved);
  }
  return bestMove;
}


int Board::miniMaxRec(int depth, double maxScore) {
  // Base case: if depth is zero, return 
  if (depth == 0) {
    return maxScore;
  }

  // GenAllValidMoves, iterate through, and find best score and return that
  std::vector<std::tuple<int, int>> moves = this->genAllValidMoves(this->turn);

  // if vector is empty, this is checkmate or a draw
  if (moves.size() == 0) {
    if (this->checkForChecks(this->turn == 'w'? this->wKingPos : this->bKingPos, this->turn)) {
      return INT_MAX;
    }
    else {
      return 0;
    }
  }
  
  int bestScore = INT_MIN;
  for (std::tuple<int, int> move : moves) {
    int pieceMoved = this->board[std::get<0>(move)];
    int pieceAtI2 = this->board[std::get<1>(move)];
    this->makeMove(move);
    int score = this->miniMaxRec(depth - 1, -1 * this->evaluate());
    if (score > bestScore) {
      bestScore = score;
    }
    this->unmakeMove(move, pieceAtI2, pieceMoved);
  }
  return -1 * bestScore;
}

