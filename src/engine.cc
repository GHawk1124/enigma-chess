#include "board.h"

#include <array>
#include <unordered_map>
#include <climits>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <set>

double Board::evaluate() {
  // std::unordered_map<int, double> wPawnSquareToBonus = {{}}
  std::set diagonalSquares = {0, 7, 9, 14, 18, 21, 27, 28, 35, 36, 42, 45, 49, 54, 56, 63};
  std::set center4Squares = {27, 28, 35, 36};
  std::set center16Squares = {18, 19, 20, 21, 26, 27, 28, 29, 34, 35, 36, 37, 42, 43, 44, 45};
  int numPieces = 0;
  int turnSwap = turn == 'w'? 1 : -1;
  double score = 0;
  for (int i = 0; i < 64; i++) {
    switch (this->board[i]) {
      case whitePawn:
        numPieces++;
        if ((i / 8) == 3) {
          score += 1.2;
        } else if (center4Squares.count(i) == 1) {
          score += 1.5;
        } else if ((i / 8) == 6) {
          score += 1.9;
        } else {
          score += 1;
        }
        if (center4Squares.count(i) == 1) {
          score += 0.4;
        }
        break;
      case whiteBishop:
        numPieces++;
        score += 3;
        if (diagonalSquares.count(i) == 1) {
          score += 0.5;
        }
        if (center16Squares.count(i) == 1) {
          score += 0.2;
        }
        break;
      case whiteKnight:
        numPieces++;
        score += 3;
        if (center16Squares.count(i) == 1) {
          score += 0.5;
        }
        break;
      case whiteRook:
        numPieces++;
        score += 5;
        break;
      case whiteQueen:
        numPieces++;
        score += 11;
        break;
      case whiteKing:
        if (i == 58) {
          score += 0.2;
        } else if (i == 62) {
          score += 0.5;
        } else if (i == 60) {
          score += 0.1;
        } else {
          score -= 0.3;
        }
      case blackPawn:
        numPieces++;
        if ((i / 8) == 2) {
          score -= 1.3;
        } else if (center4Squares.count(i) == 1) {
          score -= 1.5;
        } else if ((i / 8) == 6) {
          score -= 1.9;
        } else {
          score -= 1;
        }
        if (center4Squares.count(i) == 1) {
          score -= 0.4;
        }
        break;
      case blackBishop:
        numPieces++;
        score -= 3;
        if (diagonalSquares.count(i) == 1) {
          score -= 0.5;
        }
        if (center16Squares.count(i) == 1) {
          score -= 0.2;
        }
        break;
      case blackKnight:
        numPieces++;
        score -= 3;
        if (center16Squares.count(i) == 1) {
          score -= 0.5;
        }
        break;
      case blackRook:
        numPieces++;
        score -= 5;
        break;
      case blackQueen:
        numPieces++;
        score -= 11;
        break;
      case blackKing:
        if (i == 2) {
          score -= 0.2;
        } else if (i == 6) {
          score -= 0.5;
        } else if (i == 4) {
          score -= 0.1;
        } else {
          score += 0.3;
        }
    }
  }
  double numPiecesMultiplier = double(double(numPieces) / 100);
  return turnSwap * score + numPiecesMultiplier;
}


// Iterate through the first depth (all of black's possible moves) and call the recursive function on each of these moves
// This will give a score to all of these moves which you can then take the best score and therefore best move
// In this way you can keep track of which move to make (a number by itself is meaningless) I don't know a better way to do this

std::tuple<int, int, int> Board::runMiniMax(int depth) {
  // auto startTime = std::chrono::steady_clock::now();
  std::vector<std::tuple<int, int, int>> bestMoves;
  std::vector<std::tuple<int, int, int>> possibleMoves = this->genAllValidMoves(this->turn);
  possibleMoves = this->orderMoves(possibleMoves);
  for (int k = 0; k < depth; k++) {
    bestMoves.push_back(this->miniMax(k, possibleMoves));



    // Takes the move that is currently best (at a smaller depth) and moves it to the front
    // of the possible moves vector so that the alpha beta pruning is more effective
    // (this is assuming the best move at a smaller depth is actually a good move)
    for (int i = 0; i < possibleMoves.size(); i++) {
      if (bestMoves[i] == possibleMoves[i]) {
        std::tuple<int, int, int> temp = possibleMoves[i];
        for (int j = i; j > 0; j--) {
          possibleMoves[j] = possibleMoves[j - 1];
        }
        possibleMoves[0] = temp;
      }
    }



  }
  return bestMoves[bestMoves.size() - 1];
}

std::tuple<int, int, int> Board::miniMax(int depth, std::vector<std::tuple<int, int, int>> possibleMoves) {
  double bestMoveScore = (double)INT_MIN;
  std::tuple<int, int, int> bestMove = possibleMoves[0];

  for (std::tuple<int, int, int> move : possibleMoves) {
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
  std::vector<std::tuple<int, int, int>> moves = this->genAllValidMoves(this->turn);
  moves = this->orderMoves(moves);

  // if vector is empty, this is checkmate or a draw
  if (moves.size() == 0) {
    if (this->checkForChecks(this->turn == 'w'? this->wKingPos : this->bKingPos, this->turn)) {
      return (double)INT_MIN;
    }
    else {
      return 0;
    }
  }
  
  for (std::tuple<int, int, int> move : moves) {
    int pieceMoved = this->board[std::get<0>(move)];
    int pieceAtI2 = this->board[std::get<1>(move)];
    this->makeMove(move);
    double score = -1 * this->miniMaxRec(depth - 1, -beta, -alpha);
    this->unmakeMove(move, pieceAtI2, pieceMoved);
    if (score >= beta) {
      return beta;
    }
    if (score >= alpha) {
      alpha = score;
    }
  }
  return alpha;
}


// Sorts the move vector by the third int in decreasing order, this value representing a score of how 'good' the move might be
std::vector<std::tuple<int, int, int>> Board::orderMoves(std::vector<std::tuple<int, int, int>> moves) {

  auto cmp = [](const auto& a, const auto& b) {
    return std::get<2>(a) > std::get<2>(b);
  };
  std::sort(moves.begin(), moves.end(), cmp);

  return moves;
}