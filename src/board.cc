#include "board.h"

#include <iostream>
#include <unordered_map>

#define NOT_COL_1(x) (x % 8 != 0)
#define NOT_COL_8(x) (x % 8 != 7)

Board::Board() {}

std::string Board::getInput() {
  std::string input;
  std::cout << "Enter move: ";
  std::cin >> input;
  return input;
}

unsigned int Board::getPos(std::string input) {
  const std::unordered_map<char, int> col = {{'a', 0}, {'b', 1}, {'c', 2},
                                             {'d', 3}, {'e', 4}, {'f', 5},
                                             {'g', 6}, {'h', 7}};
  const std::unordered_map<char, int> row = {{'1', 7}, {'2', 6}, {'3', 5},
                                             {'4', 4}, {'5', 3}, {'6', 2},
                                             {'7', 1}, {'8', 0}};
  return row.at(input[1]) * 8 + col.at(input[0]);
}

void Board::printBoard() {
  const std::unordered_map<int, char> pieces = {
      {0, '-'}, {1, 'P'}, {2, 'R'}, {3, 'N'},  {4, 'B'},  {5, 'Q'}, {6, 'K'},
      {7, 'p'}, {8, 'r'}, {9, 'n'}, {10, 'b'}, {11, 'q'}, {12, 'k'}};
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      std::cout << pieces.at(board[i * 8 + j]) << " ";
    }
    std::cout << std::endl;
  }
}

void Board::makeMove(int pos, int i2) {
  bool validMove = false;
  for (auto move : this->moves) {
    if (std::get<0>(move) == pos && std::get<1>(move) == i2) {
      this->board[i2] = this->board[pos];
      this->board[pos] = 0;
      validMove = true;
      break;
    }
  }
  if (validMove) {
    if (this->turn == 'w') {
      this->turn = 'b';
    } else {
      this->turn = 'w';
    }
  } else {
    std::cout << "Invalid move!" << std::endl;
  }
}

void Board::checkPawnMoves(int pos, char turn) {
  if (turn == 'b') {
    if (pos < 16) {
      if (board[pos + 8] == 0) {
        this->moves.push_back(std::make_tuple(pos, pos + 8));
        if (board[pos + 16] == 0) {
          this->moves.push_back(std::make_tuple(pos, pos + 16));
        }
      }
    } else {
      if (board[pos + 8] == 0) {
        this->moves.push_back(std::make_tuple(pos, pos + 8));
      }
    }
    if (NOT_COL_8(pos) && board[pos + 9] < blackPawn) {
      this->moves.push_back(std::make_tuple(pos, pos + 9));
    }
    if (NOT_COL_1(pos) && board[pos + 7] < blackPawn) {
      this->moves.push_back(std::make_tuple(pos, pos + 7));
    }
  } else if (turn == 'w') {
    if (pos > 47) {
      if (board[pos - 8] == 0) {
        this->moves.push_back(std::make_tuple(pos, pos - 8));
        if (board[pos - 16] == 0) {
          this->moves.push_back(std::make_tuple(pos, pos - 16));
        }
      }
    } else {
      if (board[pos - 8] == 0) {
        this->moves.push_back(std::make_tuple(pos, pos - 8));
      }
    }
    if (NOT_COL_8(pos) && board[pos - 7] > whiteKing) {
      this->moves.push_back(std::make_tuple(pos, pos - 7));
    }
    if (NOT_COL_1(pos) && board[pos - 9] > whiteKing) {
      this->moves.push_back(std::make_tuple(pos, pos - 9));
    }
  }
}

// TODO: Refactor this function
// TODO: Take king valid?
void Board::checkRookMoves(int pos, char turn) {
  // Check row towards right
  for (int i = pos + 1; i < (int)(pos / 8) * 8 + 8; i++) {
    if (board[i] == 0) {
      this->moves.push_back(std::make_tuple(pos, i));
    } else if (turn == 'w' && board[i] > whiteKing) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else if (turn == 'b' && board[i] < blackPawn) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else {
      break;
    }
  }
  // Check row towards left
  for (int i = pos - 1; i >= (int)(pos / 8) * 8; i--) {
    if (board[i] == 0) {
      this->moves.push_back(std::make_tuple(pos, i));
    } else if (turn == 'w' && board[i] > whiteKing) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else if (turn == 'b' && board[i] < blackPawn) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else {
      break;
    }
  }
  // Check column towards top
  for (int i = pos - 8; i >= 0; i -= 8) {
    if (board[i] == 0) {
      this->moves.push_back(std::make_tuple(pos, i));
    } else if (turn == 'w' && board[i] > whiteKing) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else if (turn == 'b' && board[i] < blackPawn) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else {
      break;
    }
  }
  // Check column towards bottom
  for (int i = pos + 8; i < 64; i += 8) {
    if (board[i] == 0) {
      this->moves.push_back(std::make_tuple(pos, i));
    } else if (turn == 'w' && board[i] > whiteKing) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else if (turn == 'b' && board[i] < blackPawn) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else {
      break;
    }
  }
}

// TODO: Move if statement out of for loop
void Board::checkBishopMoves(int pos, char turn) {
  // Check diagonal towards bottom right
  for (int i = pos + 9; i < 56 + pos % 8; i += 9) {
    if (i % 8 == 7) {
      break;
    }
    if (board[i] == 0) {
      this->moves.push_back(std::make_tuple(pos, i));
    } else if (turn == 'w' && board[i] > whiteKing) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else if (turn == 'b' && board[i] < blackPawn) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else {
      break;
    }
  }
  // Check diagonal towards top right
  for (int i = pos - 7; i >= pos % 8; i -= 7) {
    if (i % 8 == 7) {
      break;
    }
    if (board[i] == 0) {
      this->moves.push_back(std::make_tuple(pos, i));
    } else if (turn == 'w' && board[i] > whiteKing) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else if (turn == 'b' && board[i] < blackPawn) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else {
      break;
    }
  }
  // Check diagonal towards bottom left
  for (int i = pos + 7; i < 56 + pos % 8; i += 7) {
    if (i % 8 == 0) {
      break;
    }
    if (board[i] == 0) {
      this->moves.push_back(std::make_tuple(pos, i));
    } else if (turn == 'w' && board[i] > whiteKing) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else if (turn == 'b' && board[i] < blackPawn) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else {
      break;
    }
  }
  // Check diagonal towards top left
  for (int i = pos - 9; i >= pos % 8; i -= 9) {
    if (i % 8 == 0) {
      break;
    }
    if (board[i] == 0) {
      this->moves.push_back(std::make_tuple(pos, i));
    } else if (turn == 'w' && board[i] > whiteKing) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else if (turn == 'b' && board[i] < blackPawn) {
      this->moves.push_back(std::make_tuple(pos, i));
      break;
    } else {
      break;
    }
  }
}

void Board::checkKnightMoves(int pos, char turn) {
  // Check top right
  if (pos % 8 < 7 && pos - 15 >= 0) {
    if (turn == 'w' && (board[pos - 15] > whiteKing || board[pos - 15] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos - 15));
    } else if (turn == 'b' &&
               (board[pos - 15] < blackPawn || board[pos - 15] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos - 15));
    }
  }
  // Check top left
  if (pos % 8 > 0 && pos - 17 >= 0) {
    if (turn == 'w' && (board[pos - 17] > whiteKing || board[pos - 17] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos - 17));
    } else if (turn == 'b' &&
               (board[pos - 17] < blackPawn || board[pos - 17] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos - 17));
    }
  }
  // Check right top
  if (pos % 8 < 6 && pos - 6 >= 0) {
    if (turn == 'w' && (board[pos - 6] > whiteKing || board[pos - 6] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos - 6));
    } else if (turn == 'b' &&
               (board[pos - 6] < blackPawn || board[pos - 6] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos - 6));
    }
  }
  // Check right bottom
  if (pos % 8 < 6 && pos + 10 < 64) {
    if (turn == 'w' && (board[pos + 10] > whiteKing || board[pos + 10] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos + 10));
    } else if (turn == 'b' &&
               (board[pos + 10] < blackPawn || board[pos + 10] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos + 10));
    }
  }
  // Check bottom right
  if (pos % 8 < 7 && pos + 17 < 64) {
    if (turn == 'w' && (board[pos + 17] > whiteKing || board[pos + 17] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos + 17));
    } else if (turn == 'b' &&
               (board[pos + 17] < blackPawn || board[pos + 17] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos + 17));
    }
  }
  // Check bottom left
  if (pos % 8 > 0 && pos + 15 < 64) {
    if (turn == 'w' && (board[pos + 15] > whiteKing || board[pos + 15] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos + 15));
    } else if (turn == 'b' &&
               (board[pos + 15] < blackPawn || board[pos + 15] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos + 15));
    }
  }
  // Check left top
  if (pos % 8 > 1 && pos - 10 >= 0) {
    if (turn == 'w' && (board[pos - 10] > whiteKing || board[pos - 10] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos - 10));
    } else if (turn == 'b' &&
               (board[pos - 10] < blackPawn || board[pos - 10] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos - 10));
    }
  }
  // Check left bottom
  if (pos % 8 > 1 && pos + 6 < 64) {
    if (turn == 'w' && (board[pos + 6] > whiteKing || board[pos + 6] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos + 6));
    } else if (turn == 'b' &&
               (board[pos + 6] < blackPawn || board[pos + 6] == 0)) {
      this->moves.push_back(std::make_tuple(pos, pos + 6));
    }
  }
}

// TODO: Move if statement out of for loop
bool Board::checkForChecks(int pos, char turn) {
  // Knight Section
  // Check top right
  if (pos % 8 < 7 && pos - 15 >= 0) {
    if (turn == 'w' && board[pos - 15] == blackKnight ||
        turn == 'b' && board[pos - 15] == whiteKnight) {
      return true;
    }
  }
  // Check top left
  if (pos % 8 > 0 && pos - 17 >= 0) {
    if (turn == 'w' && board[pos - 17] == blackKnight ||
        turn == 'b' && board[pos - 17] == whiteKnight) {
      return true;
    }
  }
  // Check right top
  if (pos % 8 < 6 && pos - 6 >= 0) {
    if (turn == 'w' && board[pos - 6] == blackKnight ||
        turn == 'b' && board[pos - 6] == whiteKnight) {
      return true;
    }
  }
  // Check right bottom
  if (pos % 8 < 6 && pos + 10 < 64) {
    if (turn == 'w' && board[pos + 10] == blackKnight ||
        turn == 'b' && board[pos + 10] == whiteKnight) {
      return true;
    }
  }
  // Check bottom right
  if (pos % 8 < 7 && pos + 17 < 64) {
    if (turn == 'w' && board[pos + 17] == blackKnight ||
        turn == 'b' && board[pos + 17] == whiteKnight) {
      return true;
    }
  }
  // Check bottom left
  if (pos % 8 > 0 && pos + 15 < 64) {
    if (turn == 'w' && board[pos + 15] == blackKnight ||
        turn == 'b' && board[pos + 15] == whiteKnight) {
      return true;
    }
  }
  // Check left top
  if (pos % 8 > 1 && pos - 10 >= 0) {
    if (turn == 'w' && board[pos - 10] == blackKnight ||
        turn == 'b' && board[pos - 10] == whiteKnight) {
      return true;
    }
  }
  // Check left bottom
  if (pos % 8 > 1 && pos + 6 < 64) {
    if (turn == 'w' && board[pos + 6] == blackKnight ||
        turn == 'b' && board[pos + 6] == whiteKnight) {
      return true;
    }
  }

  // Diagonal Section
  // Check diagonal towards bottom right
  for (int i = pos + 9; i < 64; i += 9) {
    if (i % 8 == 7) {
      break;
    }
    if (turn == 'w' && board[i] == blackBishop || board[i] == blackQueen) {
      return true;
    } else if (turn == 'b' && board[i] == whiteBishop ||
               board[i] == whiteQueen) {
      return true;
    } else {
      return false;
    }
  }
  // Check diagonal towards top right
  for (int i = pos - 7; i > 0; i -= 7) {
    if (i % 8 == 7) {
      break;
    }
    if (turn == 'w' && board[i] == blackBishop || board[i] == blackQueen) {
      return true;
    } else if (turn == 'b' && board[i] == whiteBishop ||
               board[i] == whiteQueen) {
      return true;
    } else {
      return false;
    }
  }
  // Check diagonal towards top left
  for (int i = pos - 9; i >= 0; i -= 9) {
    if (i % 8 == 0) {
      break;
    }
    if (turn == 'w' && board[i] == blackBishop || board[i] == blackQueen) {
      return true;
    } else if (turn == 'b' && board[i] == whiteBishop ||
               board[i] == whiteQueen) {
      return true;
    } else {
      return false;
    }
  }
  // Check diagonal towards bottom left
  for (int i = pos + 7; i < 63; i += 7) {
    if (i % 8 == 0) {
      break;
    }
    if (turn == 'w' && board[i] == blackBishop || board[i] == blackQueen) {
      return true;
    } else if (turn == 'b' && board[i] == whiteBishop ||
               board[i] == whiteQueen) {
      return true;
    } else {
      return false;
    }
  }

  // Horizontal Section
  // Check horizontal towards right
  for (int i = pos + 1; i < (int)(pos / 8) * 8 + 8; i++) {
    if (i % 8 == 7) {
      break;
    }
    if (turn == 'w' && board[i] == blackRook || board[i] == blackQueen) {
      return true;
    } else if (turn == 'b' && board[i] == whiteRook || board[i] == whiteQueen) {
      return true;
    } else {
      return false;
    }
  }
  // Check horizontal towards left
  for (int i = pos - 1; i >= (int)(pos / 8) * 8; i--) {
    if (i % 8 == 0) {
      break;
    }
    if (turn == 'w' && board[i] == blackRook || board[i] == blackQueen) {
      return true;
    } else if (turn == 'b' && board[i] == whiteRook || board[i] == whiteQueen) {
      return true;
    } else {
      return false;
    }
  }
  // Check vertical towards top
  for (int i = pos - 8; i >= 0; i -= 8) {
    if (turn == 'w' && board[i] == blackRook || board[i] == blackQueen) {
      return true;
    } else if (turn == 'b' && board[i] == whiteRook || board[i] == whiteQueen) {
      return true;
    } else {
      return false;
    }
  }
  // Check vertical towards bottom
  for (int i = pos + 8; i < 64; i += 8) {
    if (turn == 'w' && board[i] == blackRook || board[i] == blackQueen) {
      return true;
    } else if (turn == 'b' && board[i] == whiteRook || board[i] == whiteQueen) {
      return true;
    } else {
      return false;
    }
  }

  // Check for Pawns
  if (turn == 'w') {
    if (NOT_COL_1(pos) && pos - 9 >= 0 && board[pos - 9] == blackPawn) {
      return true;
    }
    if (NOT_COL_8(pos) && pos - 7 >= 0 && board[pos - 7] == blackPawn) {
      return true;
    }
  } else if (turn == 'b') {
    if (NOT_COL_1(pos) && pos + 7 < 64 && board[pos + 7] == whitePawn) {
      return true;
    }
    if (NOT_COL_8(pos) && pos + 9 < 64 && board[pos + 9] == whitePawn) {
      return true;
    }
  }

  // Check for Kings
  if (turn == 'w') {
    // Top Left
    if (NOT_COL_1(pos) && pos - 9 >= 0 && board[pos - 9] == blackKing) {
      return true;
    }
    // Top Right
    if (NOT_COL_8(pos) && pos - 7 >= 0 && board[pos - 7] == blackKing) {
      return true;
    }
    // Top
    if (pos - 8 >= 0 && board[pos - 8] == blackKing) {
      return true;
    }
    // Bottom Left
    if (NOT_COL_1(pos) && pos + 7 < 64 && board[pos + 7] == blackKing) {
      return true;
    }
    // Bottom Right
    if (NOT_COL_8(pos) && pos + 9 < 64 && board[pos + 9] == blackKing) {
      return true;
    }
    // Bottom
    if (pos + 8 < 64 && board[pos + 8] == blackKing) {
      return true;
    }
    // Right
    if (NOT_COL_8(pos) && board[pos + 1] == blackKing) {
      return true;
    }
    // Left
    if (NOT_COL_1(pos) && board[pos - 1] == blackKing) {
      return true;
    }
  } else if (turn == 'b') {
    // Top Left
    if (NOT_COL_1(pos) && pos - 9 >= 0 && board[pos - 9] == whiteKing) {
      return true;
    }
    // Top Right
    if (NOT_COL_8(pos) && pos - 7 >= 0 && board[pos - 7] == whiteKing) {
      return true;
    }
    // Top
    if (pos - 8 >= 0 && board[pos - 8] == whiteKing) {
      return true;
    }
    // Bottom Left
    if (NOT_COL_1(pos) && pos + 7 < 64 && board[pos + 7] == whiteKing) {
      return true;
    }
    // Bottom Right
    if (NOT_COL_8(pos) && pos + 9 < 64 && board[pos + 9] == whiteKing) {
      return true;
    }
    // Bottom
    if (pos + 8 < 64 && board[pos + 8] == whiteKing) {
      return true;
    }
    // Right
    if (NOT_COL_8(pos) && board[pos + 1] == whiteKing) {
      return true;
    }
    // Left
    if (NOT_COL_1(pos) && board[pos - 1] == whiteKing) {
      return true;
    }
  }
  return false;
}

void Board::checkKingMoves(int pos, char turn) {
  if (turn == 'w') {
    // Top Left
    if (NOT_COL_1(pos) && pos - 9 >= 0 &&
        (board[pos - 9] == 0 || board[pos - 9] > whiteKing)) {
      if (checkForChecks(pos - 9, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 9));
      }
    }
    // Top Right
    if (NOT_COL_8(pos) && pos - 7 >= 0 &&
        (board[pos - 7] == 0 || board[pos - 7] > whiteKing)) {
      if (checkForChecks(pos - 7, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 7));
      }
    }
    // Top
    if (pos - 8 >= 0 && (board[pos - 8] == 0 || board[pos - 8] > whiteKing)) {
      if (checkForChecks(pos - 8, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 8));
      }
    }
    // Bottom Left
    if (NOT_COL_1(pos) && pos + 7 < 64 &&
        (board[pos + 7] == 0 || board[pos + 7] > whiteKing)) {
      if (checkForChecks(pos + 7, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 7));
      }
    }
    // Bottom Right
    if (NOT_COL_8(pos) && pos + 9 < 64 &&
        (board[pos + 9] == 0 || board[pos + 9] > whiteKing)) {
      if (checkForChecks(pos + 9, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 9));
      }
    }
    // Bottom
    if (pos + 8 < 64 && (board[pos + 8] == 0 || board[pos + 8] > whiteKing)) {
      if (!checkForChecks(pos + 8, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 8));
      }
    }
    // Right
    if (NOT_COL_8(pos) && (board[pos + 1] == 0 || board[pos + 1] > whiteKing)) {
      if (!checkForChecks(pos + 1, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 1));
      }
    }
    // Left
    if (NOT_COL_1(pos) && (board[pos - 1] == 0 || board[pos - 1] > whiteKing)) {
      if (!checkForChecks(pos - 1, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 1));
      }
    }
  } else if (turn == 'b') {
    // Top Left
    if (NOT_COL_1(pos) && pos - 9 >= 0 &&
        (board[pos - 9] == 0 || board[pos - 9] < blackKing)) {
      if (!checkForChecks(pos - 9, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 9));
      }
    }
    // Top Right
    if (NOT_COL_8(pos) && pos - 7 >= 0 &&
        (board[pos - 7] == 0 || board[pos - 7] < blackKing)) {
      if (!checkForChecks(pos - 7, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 7));
      }
    }
    // Top
    if (pos - 8 >= 0 && (board[pos - 8] == 0 || board[pos - 8] < blackKing)) {
      if (!checkForChecks(pos - 8, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 8));
      }
    }
    // Bottom Left
    if (NOT_COL_1(pos) && pos + 7 < 64 &&
        (board[pos + 7] == 0 || board[pos + 7] < blackKing)) {
      if (!checkForChecks(pos + 7, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 7));
      }
    }
    // Bottom Right
    if (NOT_COL_8(pos) && pos + 9 < 64 &&
        (board[pos + 9] == 0 || board[pos + 9] < blackKing)) {
      if (!checkForChecks(pos + 9, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 9));
      }
    }
    // Bottom
    if (pos + 8 < 64 && (board[pos + 8] == 0 || board[pos + 8] < blackKing)) {
      if (!checkForChecks(pos + 8, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 8));
      }
    }
    // Right
    if (NOT_COL_8(pos) && (board[pos + 1] == 0 || board[pos + 1] < blackKing)) {
      if (!checkForChecks(pos + 1, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 1));
      }
    }
    // Left
    if (NOT_COL_1(pos) && (board[pos - 1] == 0 || board[pos - 1] < blackKing)) {
      if (!checkForChecks(pos - 1, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 1));
      }
    }
  }
}

void Board::genValidMoves(int pos, char turn) {
  if (turn == 'w') {
    switch (this->board[pos]) {
    case 1:
      // Pawn
      // TODO: Add en passant
      this->checkPawnMoves(pos, turn);
      break;
    case 2:
      // Rook
      this->checkRookMoves(pos, turn);
      break;
    case 3:
      // Knight
      this->checkKnightMoves(pos, turn);
      break;
    case 4:
      // Bishop
      this->checkBishopMoves(pos, turn);
      break;
    case 5:
      // Queen
      this->checkRookMoves(pos, turn);
      this->checkBishopMoves(pos, turn);
      break;
    case 6:
      // King
      this->checkKingMoves(pos, turn);
      break;
    }
  } else if (turn == 'b') {
    switch (this->board[pos]) {
    case 7:
      // Black Pawn
      // TODO: Add en passant
      this->checkPawnMoves(pos, turn);
      break;
    case 8:
      // Black Rook
      this->checkRookMoves(pos, turn);
      break;
    case 9:
      // Black Knight
      this->checkKnightMoves(pos, turn);
      break;
    case 10:
      // Black Bishop
      this->checkBishopMoves(pos, turn);
      break;
    case 11:
      // Black Queen
      this->checkRookMoves(pos, turn);
      this->checkBishopMoves(pos, turn);
      break;
    case 12:
      // Black King
      this->checkKingMoves(pos, turn);
      break;
    default:
      std::cout << "Invalid move" << std::endl;
      break;
    }
  }
}

void Board::clearBoard() {}