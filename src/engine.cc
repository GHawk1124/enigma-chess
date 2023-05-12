#include "board.h"

#include <array>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <set>

double Board::evaluate() {
  // std::unordered_map<int, double> wPawnSquareToBonus = {{}}
  std::set diagonalSquares = {0, 7, 9, 14, 18, 21, 27, 28, 35, 36, 42, 45, 49, 54, 56, 63};
  std::set center4Squares = {27, 28, 35, 36};
  std::set center16Squares = {18, 19, 20, 21, 26, 27, 28, 29, 34, 35, 36, 37, 42, 43, 44, 45};
  int turnSwap = turn == 'w'? 1 : -1;
  double score = 0;
  for (int i = 0; i < 64; i++) {
    switch (this->board[i]) {
      case whitePawn:
        if ((i / 8) == 4) {
          score += 1.4;
        } else if ((i / 8) == 5) {
          score += 1.3;
        } else {
          score += 1;
        }
        if (center4Squares.count(i) == 1) {
          score += 0.4;
        }
        break;
      case whiteBishop:
        score += 3;
        if (diagonalSquares.count(i) == 1) {
          score += 0.5;
        }
        if (center16Squares.count(i) == 1) {
          score += 0.2;
        }
        break;
      case whiteKnight:
        score += 3;
        if (center16Squares.count(i) == 1) {
          score += 0.5;
        }
        break;
      case whiteRook:
        score += 5;
        break;
      case whiteQueen:
        score += 11;
        break;
      case whiteKing:
        if (i == 58) {
          score += 0.2;
        } else if (i == 62) {
          score += 0.5;
        }
      case blackPawn:
        if ((i / 8) == 3) {
          score -= 1.5;
        } else if ((i / 8) == 2) {
          score -= 1.3;
        } else {
          score -= 1;
        }
        if (center4Squares.count(i) == 1) {
          score -= 0.4;
        }
        break;
      case blackBishop:
        score -= 3;
        if (diagonalSquares.count(i) == 1) {
          score -= 0.5;
        }
        if (center16Squares.count(i) == 1) {
          score -= 0.2;
        }
        break;
      case blackKnight:
        score -= 3;
        if (center16Squares.count(i) == 1) {
          score -= 0.5;
        }
        break;
      case blackRook:
        score -= 5;
        break;
      case blackQueen:
        score -= 11;
        break;
      case blackKing:
        if (i == 2) {
          score -= 0.2;
        } else if (i == 6) {
          score -= 0.5;
        }
    }
  }
  return turnSwap * score;
}


// Iterate through the first depth (all of black's possible moves) and call the recursive function on each of these moves
// This will give a score to all of these moves which you can then take the best score and therefore best move
// In this way you can keep track of which move to make (a number by itself is meaningless) I don't know a better way to do this
std::tuple<int, int> Board::miniMax(int depth) {
  std::vector<std::tuple<int, int>> possibleMoves = this->genAllValidMoves(this->turn);
  double bestMoveScore = (double)INT_MIN;
  std::tuple<int, int> bestMove = possibleMoves[0];

  for (std::tuple<int, int> move : possibleMoves) {
    int pieceMoved = this->board[std::get<0>(move)];
    int pieceAtI2 = this->board[std::get<1>(move)];
    this->makeMove(move);
    double moveScore = -1 * this->miniMaxRec(depth, INT_MIN, -bestMoveScore);
    if (moveScore > bestMoveScore) {
      bestMoveScore = moveScore;
      bestMove = move;
    }
    this->unmakeMove(move, pieceAtI2, pieceMoved);
  }
  
  return bestMove;
}


double Board::miniMaxRec(int depth, double alpha, double beta) {
  // Base case: if depth is zero, return 
  if (depth == 0) {
    return this->evaluate();
  }

  // GenAllValidMoves, iterate through, and find best score and return that
  std::vector<std::tuple<int, int>> moves = this->genAllValidMoves(this->turn);

  // if vector is empty, this is checkmate or a draw
  if (moves.size() == 0) {
    if (this->checkForChecks(this->turn == 'w'? this->wKingPos : this->bKingPos, this->turn)) {
      return (double)INT_MIN;
    }
    else {
      return 0;
    }
  }
  
  for (std::tuple<int, int> move : moves) {
    int pieceMoved = this->board[std::get<0>(move)];
    int pieceAtI2 = this->board[std::get<1>(move)];
    this->makeMove(move);
    double score = -1 * this->miniMaxRec(depth - 1, -beta, -alpha);
    this->unmakeMove(move, pieceAtI2, pieceMoved);
    if (score >= beta) {
      return beta;
    }
    if (score > alpha) {
      alpha = score;
    }
  }
  return alpha;
}

