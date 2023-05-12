#include "board.h"

#include <array>
#include <cctype>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstring>

#define NOT_COL_1(x) (x % 8 != 0)
#define NOT_COL_8(x) (x % 8 != 7)

Board::Board() {}

std::array<unsigned int, 64> Board::getBoard() { return this->board; }

void Board::setBoard(std::array<unsigned int, 64> board) {
  this->board = board;
}

std::array<unsigned int, 64> Board::decode_fen(const std::string &fen_string) {
  std::array<unsigned int, 64> board = {0};
  int rank = 0;
  int file = 0;
  bool whites_turn = true;
  bool white_kingside_castle = false;
  bool white_queenside_castle = false;
  bool black_kingside_castle = false;
  bool black_queenside_castle = false;

  for (int i = 0; i < fen_string.length(); i++) {
    auto c = fen_string[i];
    if (c == '/') {
      rank++;
      file = 0;
    } else if (c >= '1' && c <= '8') {
      file += c - '0';
    } else if (c == ' ') {
      // next part of the FEN string
      if (fen_string[i + 1] == 'b') {
        whites_turn = false;
      }

      // skip the next part of the FEN string (en passant)
      size_t i = fen_string.find(' ', fen_string.find(' ') + 1);
      std::string castle_str = fen_string.substr(i + 1, 4);
      white_kingside_castle = castle_str.find('K') != std::string::npos;
      white_queenside_castle = castle_str.find('Q') != std::string::npos;
      black_kingside_castle = castle_str.find('k') != std::string::npos;
      black_queenside_castle = castle_str.find('q') != std::string::npos;

      break;
    } else {
      int pieceValue;
      switch (c) {
      case 'P':
        pieceValue = whitePawn;
        break;
      case 'N':
        pieceValue = whiteKnight;
        break;
      case 'B':
        pieceValue = whiteBishop;
        break;
      case 'R':
        pieceValue = whiteRook;
        break;
      case 'Q':
        pieceValue = whiteQueen;
        break;
      case 'K':
        pieceValue = whiteKing;
        break;
      case 'p':
        pieceValue = blackPawn;
        break;
      case 'n':
        pieceValue = blackKnight;
        break;
      case 'b':
        pieceValue = blackBishop;
        break;
      case 'r':
        pieceValue = blackRook;
        break;
      case 'q':
        pieceValue = blackQueen;
        break;
      case 'k':
        pieceValue = blackKing;
        break;
      default:
        // Invalid FEN string
        return {};
      }
      board[rank * 8 + file] = pieceValue;
      file++;
    }
  }

  // set turn flag
  this->turn = whites_turn ? 'w' : 'b';

  // set castle flags
  this->white_kingside_castle = white_kingside_castle;
  this->white_queenside_castle = white_queenside_castle;
  this->black_kingside_castle = black_kingside_castle;
  this->black_queenside_castle = black_queenside_castle;

  return board;
}

std::array<unsigned int, 64> Board::decode_fen_c(const char *fen_string) {
  return decode_fen(std::string(fen_string));
}

std::string Board::encode_fen(const std::array<unsigned int, 64> &board) {
  std::string fen_string;
  int empty = 0;
  for (int rank = 0; rank <= 7; rank++) {
    for (int file = 0; file < 8; file++) {
      int piece = board[rank * 8 + file];
      if (piece == Empty) {
        empty++;
      } else {
        if (empty > 0) {
          fen_string += std::to_string(empty);
          empty = 0;
        }
        switch (piece) {
        case whitePawn:
          fen_string += 'P';
          break;
        case whiteKnight:
          fen_string += 'N';
          break;
        case whiteBishop:
          fen_string += 'B';
          break;
        case whiteRook:
          fen_string += 'R';
          break;
        case whiteQueen:
          fen_string += 'Q';
          break;
        case whiteKing:
          fen_string += 'K';
          break;
        case blackPawn:
          fen_string += 'p';
          break;
        case blackKnight:
          fen_string += 'n';
          break;
        case blackBishop:
          fen_string += 'b';
          break;
        case blackRook:
          fen_string += 'r';
          break;
        case blackQueen:
          fen_string += 'q';
          break;
        case blackKing:
          fen_string += 'k';
          break;
        }
      }
    }
    if (empty > 0) {
      fen_string += std::to_string(empty);
      empty = 0;
    }
    if (rank < 7) {
      fen_string += '/';
    }
  }

  fen_string += ' ';
  fen_string += this->turn;

  fen_string += ' ';
  if (this->white_kingside_castle) {
    fen_string += 'K';
  }
  if (this->white_queenside_castle) {
    fen_string += 'Q';
  }
  if (this->black_kingside_castle) {
    fen_string += 'k';
  }
  if (this->black_queenside_castle) {
    fen_string += 'q';
  }
  if (!this->white_kingside_castle && !this->white_queenside_castle &&
      !this->black_kingside_castle && !this->black_queenside_castle) {
    fen_string += '-';
  }

  fen_string += " - 0 1";
  return fen_string;
}

char *Board::encode_fen_c(const std::array<unsigned int, 64> &board) {
  std::string fen_string = encode_fen(board);
  char *fen = new char[fen_string.length() + 1];
  std::strcpy(fen, fen_string.c_str());
  return fen;
}

std::string Board::getInput() {
  std::string input;
  std::cout << (turn == 'w' ? "White " : "Black ") << "to move: ";
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

void Board::printValidMoves(std::vector<std::tuple<int, int>> inputMovesVector) {
  const std::unordered_map<int, char> col = {{0, 'a'}, {1, 'b'}, {2, 'c'},
                                             {3, 'd'}, {4, 'e'}, {5, 'f'},
                                             {6, 'g'}, {7, 'h'}};
  const std::unordered_map<int, char> row = {{7, '1'}, {6, '2'}, {5, '3'},
                                             {4, '4'}, {3, '5'}, {2, '6'},
                                             {1, '7'}, {0, '8'}};

  std::cout << "Valid moves: ";
  for (auto move : inputMovesVector) {
    int i1 = std::get<0>(move);
    int i2 = std::get<1>(move);
    std::string result;
    result.push_back(col.at(i1 % 8));
    result.push_back(row.at(i1 / 8));
    result.push_back(col.at(i2 % 8));
    result.push_back(row.at(i2 / 8));
    std::cout << result + " ";
  }
  std::cout << std::endl;
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
  std::cout << std::endl;
}

void Board::makeMove(std::tuple<int, int> move) {
  int pos = std::get<0>(move);
  int i2 = std::get<1>(move);
  bool validMove = false;
  // First check for draw by 50 moves, checkmate, and stalemate
  if (movesSinceLastPawnMovedAndPieceTaken >= 50) {
    moves.clear();
    std::get<1>(checkmateDrawInformation) = true;
  } else if (moves.size() == 0 &&
             checkForChecks(pos, turn == 'w' ? bKingPos : wKingPos)) {
    std::get<0>(checkmateDrawInformation) = true;
  } else if (moves.size() == 0) {
    std::get<1>(checkmateDrawInformation) = true;
  }
  movesSinceLastPawnMovedAndPieceTaken++;

  // Special case for castling
  if (i2 == pos - 2 &&
      (this->board[pos] == whiteKing || this->board[pos] == blackKing)) {
    this->board[pos - 1] = this->board[pos - 4];
    this->board[pos - 4] = 0;
  } else if (i2 == pos + 2 && (this->board[pos] == whiteKing ||
                                this->board[pos] == blackKing)) {
    this->board[pos + 1] = this->board[pos + 3];
    this->board[pos + 3] = 0;
  }

  // special case for en passant
  // I THINK THIS NEEDS EDITING I THINK THE SECOND IF STATEMENT SHOULD BE INSIDE THE FIRST???
  // if ((board[pos] == whitePawn) || (board[pos] == blackPawn)) {
  //   if ((abs(i2 - pos) == 7 || abs(i2 - pos) == 9) && board[i2] == 0) {
  //     this->board[std::get<1>(enPassant)] = 0;
  //     std::get<0>(enPassantMove) = true;
  //     std::get<1>(enPassantMove) = this->board[std::get<1>(enPassant)];
  //   } else {
  //     this->enPassantMove = std::make_tuple(false, -1);
  //   }
  // }

  // Make the move (check if something is taken for 50 move draw)
  if (this->board[i2] > 0) {
    movesSinceLastPawnMovedAndPieceTaken = 0;
  }
  this->board[i2] = this->board[pos];
  this->board[pos] = 0;

  // Check if piece moved was a rook or king (for castling) or a pawn (for
  // pawn promotion)
  int piece = this->board[i2];
  if (piece == whiteRook) {
    if (pos == 56) {
      this->queensideWRookMoved = true;
    } else if (pos == 63) {
      this->kingsideWRookMoved = true;
    }
  } else if (piece == blackRook) {
    if (pos == 0) {
      this->queensideBRookMoved = true;
    } else if (pos == 7) {
      this->kingsideBRookMoved = true;
    }
  } else if (piece == whiteKing) {
    this->wKingPos = i2;
    this->wKingMoved = true;
  } else if (piece == blackKing) {
    this->bKingPos = i2;
    this->bKingMoved = true;
  } else if (piece == whitePawn && i2 < 8 ||
              piece == blackPawn && i2 > 55) {
    this->board[i2] = turn == 'w'? whiteQueen : blackQueen;}
  // If a pawn was moved up two spaces, checks for en passant possible on next move and sets the variable accordingly
  // } else if (piece == whitePawn && pos == i2 + 16) {
  //   if ((board[i2 + 1] == blackPawn && NOT_COL_8(i2)) ||
  //       (board[i2 - 1] == blackPawn && NOT_COL_1(i2))) {
  //     std::get<0>(enPassant) = true;
  //     std::get<1>(enPassant) = i2;
  //   }
  // } else if (piece == blackPawn && pos == i2 - 16) {
  //   if ((board[i2 + 1] == whitePawn && NOT_COL_8(i2)) ||
  //       (board[i2 - 1] == whitePawn && NOT_COL_1(i2))) {
  //     std::get<0>(enPassant) = true;
  //     std::get<1>(enPassant) = i2;
  //   }
  // } else {
  //   this->enPassant = std::make_tuple(false, -1);
  // }
  // Not sure what these were for
  // std::string fenString = this->encode_fen(board);
  // int temp = boardConfigurations;
  if (this->turn == 'w') {
    this->turn = 'b';
  } else {
    this->turn = 'w';
  }
}


// doesn't handle en passant
void Board::unmakeMove(std::tuple<int, int> move, int i2BeforeMove, int pieceMoved) {
  int pos = std::get<0>(move);
  int i2 = std::get<1>(move);
  board[pos] = board[i2];
  board[i2] = i2BeforeMove;
  // Case for castling
  if (i2 == pos - 2 &&
      (this->board[pos] == whiteKing || this->board[pos] == blackKing)) {
    this->board[pos - 4] = this->board[pos - 1];
    this->board[pos - 1] = 0;
  } else if (i2 == pos + 2 && (this->board[pos] == whiteKing ||
                                this->board[pos] == blackKing)) {
    this->board[pos + 3] = this->board[pos + 1];
    this->board[pos + 1] = 0;
  }
  // Case for pawn promotion
  if (i2 / 8 == 0 && pieceMoved == whitePawn) {
    board[pos] = whitePawn;
  } else if (i2 / 8 == 7 && pieceMoved == blackPawn) {
    board[pos] = blackPawn;
  }
  // Case for en passant
  // if (std::get<0>(enPassantMove)) {
  //   this->board[std::get<1>(enPassantMove)] = this->turn == 'w'? whitePawn : blackPawn;
  // }
  // Update kingPos if necessary
  if (board[pos] == whiteKing) {
    this->wKingPos = pos;
  } else if (board[pos] == blackKing) {
    this->bKingPos = pos;
  }
  if (this->turn == 'w') {
    this->turn = 'b';
  } else {
    this->turn = 'w';
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
    if (NOT_COL_8(pos) && board[pos + 9] < blackPawn && board[pos + 9] != 0) {
      this->moves.push_back(std::make_tuple(pos, pos + 9));
    }
    if (NOT_COL_1(pos) && board[pos + 7] < blackPawn && board[pos + 7] != 0) {
      this->moves.push_back(std::make_tuple(pos, pos + 7));
    }
    // if (std::get<0>(enPassant) == true &&
    //     abs(std::get<1>(enPassant) - pos) == 1) {
    //   this->moves.push_back(std::make_tuple(pos, std::get<1>(enPassant) + 8));
    // }
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
    // if (std::get<0>(enPassant) == true &&
    //     abs(std::get<1>(enPassant) - pos) == 1) {
    //   this->moves.push_back(std::make_tuple(pos, std::get<1>(enPassant) - 8));
    // }
  }
}

// TODO: Refactor this function
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
  for (int i = pos + 9; i < 64; i += 9) {
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
    } else if (board[i] > 0) {
      break;
    }
  }
  // Check diagonal towards top right
  for (int i = pos - 7; i >= 0; i -= 7) {
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
    } else if (board[i] > 0) {
      break;
    }
  }
  // Check diagonal towards bottom left
  for (int i = pos + 7; i < 64; i += 7) {
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
    } else if (board[i] > 0) {
      break;
    }
  }
  // Check diagonal towards top left
  for (int i = pos - 9; i >= 0; i -= 9) {
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
    } else if (board[i] > 0) {
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

// Checks if a square on the board (pos) is currently in check
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
    if (i % 8 == 0) {
      break;
    }
    if (turn == 'w' && (board[i] == blackBishop || board[i] == blackQueen)) {
      return true;
    } else if (turn == 'b' &&
               (board[i] == whiteBishop || board[i] == whiteQueen)) {
      return true;
    } else if (board[i] > 0) {
      break;
    }
  }
  // Check diagonal towards top right
  for (int i = pos - 7; i >= 0; i -= 7) {
    if (i % 8 == 0) {
      break;
    }
    if (turn == 'w' && (board[i] == blackBishop || board[i] == blackQueen)) {
      return true;
    } else if (turn == 'b' &&
               (board[i] == whiteBishop || board[i] == whiteQueen)) {
      return true;
    } else if (board[i] > 0) {
      break;
    }
  }
  // Check diagonal towards top left
  for (int i = pos - 9; i >= 0; i -= 9) {
    if (i % 8 == 7) {
      break;
    }
    if (turn == 'w' && (board[i] == blackBishop || board[i] == blackQueen)) {
      return true;
    } else if (turn == 'b' &&
               (board[i] == whiteBishop || board[i] == whiteQueen)) {
      return true;
    } else if (board[i] > 0) {
      break;
    }
  }
  // Check diagonal towards bottom left
  for (int i = pos + 7; i < 64; i += 7) {
    if (i % 8 == 7) {
      break;
    }
    if (turn == 'w' && (board[i] == blackBishop || board[i] == blackQueen)) {
      return true;
    } else if (turn == 'b' &&
               (board[i] == whiteBishop || board[i] == whiteQueen)) {
      return true;
    } else if (board[i] > 0) {
      break;
    }
  }

  // Horizontal Section
  // Check horizontal towards right
  for (int i = pos + 1; i < (int)(pos / 8) * 8 + 8; i++) {
    if (turn == 'w' && (board[i] == blackRook || board[i] == blackQueen)) {
      return true;
    } else if (turn == 'b' &&
               (board[i] == whiteRook || board[i] == whiteQueen)) {
      return true;
    } else if (board[i] > 0) {
      break;
    }
  }
  // Check horizontal towards left
  for (int i = pos - 1; i >= (int)(pos / 8) * 8; i--) {
    if (turn == 'w' && (board[i] == blackRook || board[i] == blackQueen)) {
      return true;
    } else if (turn == 'b' &&
               (board[i] == whiteRook || board[i] == whiteQueen)) {
      return true;
    } else if (board[i] > 0) {
      break;
    }
  }
  // Check vertical towards top
  for (int i = pos - 8; i >= 0; i -= 8) {
    if (turn == 'w' && (board[i] == blackRook || board[i] == blackQueen)) {
      return true;
    } else if (turn == 'b' &&
               (board[i] == whiteRook || board[i] == whiteQueen)) {
      return true;
    } else if (board[i] > 0) {
      break;
    }
  }
  // Check vertical towards bottom
  for (int i = pos + 8; i < 64; i += 8) {
    if (turn == 'w' && (board[i] == blackRook || board[i] == blackQueen)) {
      return true;
    } else if (turn == 'b' &&
               (board[i] == whiteRook || board[i] == whiteQueen)) {
      return true;
    } else if (board[i] > 0) {
      break;
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

  // Temporarily remove the king from the board because it might block the path of checks (a copy will remain
  // behind once the king moves)
  int kingRemoved = board[pos];
  board[pos] = 0;

  if (turn == 'w') {
    // Top Left
    if (NOT_COL_1(pos) && pos - 9 >= 0 &&
        (board[pos - 9] == 0 || board[pos - 9] > whiteKing)) {
      if (!checkForChecks(pos - 9, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 9));
      }
    }
    // Top Right
    if (NOT_COL_8(pos) && pos - 7 >= 0 &&
        (board[pos - 7] == 0 || board[pos - 7] > whiteKing)) {
      if (!checkForChecks(pos - 7, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 7));
      }
    }
    // Top
    if (pos - 8 >= 0 && (board[pos - 8] == 0 || board[pos - 8] > whiteKing)) {
      if (!checkForChecks(pos - 8, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 8));
      }
    }
    // Bottom Left
    if (NOT_COL_1(pos) && pos + 7 < 64 &&
        (board[pos + 7] == 0 || board[pos + 7] > whiteKing)) {
      if (!checkForChecks(pos + 7, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 7));
      }
    }
    // Bottom Right
    if (NOT_COL_8(pos) && pos + 9 < 64 &&
        (board[pos + 9] == 0 || board[pos + 9] > whiteKing)) {
      if (!checkForChecks(pos + 9, turn)) {
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
    // Castling
    if (!wKingMoved) {
      if (!this->queensideWRookMoved && this->board[57] == Empty &&
          this->board[58] == Empty && this->board[59] == Empty &&
          !checkForChecks(57, turn) && !checkForChecks(58, turn) &&
          !checkForChecks(59, turn) && !checkForChecks(60, turn)) {
        this->moves.push_back(std::make_tuple(60, 58));
      }
      if (!this->kingsideWRookMoved && this->board[61] == Empty &&
          this->board[62] == Empty && !checkForChecks(60, turn) &&
          !checkForChecks(61, turn) && !checkForChecks(62, turn)) {
        this->moves.push_back(std::make_tuple(60, 62));
      }
    }
  } else if (turn == 'b') {
    // Top Left
    if (NOT_COL_1(pos) && pos - 9 >= 0 &&
        (board[pos - 9] == 0 || board[pos - 9] < blackPawn)) {
      if (!checkForChecks(pos - 9, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 9));
      }
    }
    // Top Right
    if (NOT_COL_8(pos) && pos - 7 >= 0 &&
        (board[pos - 7] == 0 || board[pos - 7] < blackPawn)) {
      if (!checkForChecks(pos - 7, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 7));
      }
    }
    // Top
    if (pos - 8 >= 0 && (board[pos - 8] == 0 || board[pos - 8] < blackPawn)) {
      if (!checkForChecks(pos - 8, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 8));
      }
    }
    // Bottom Left
    if (NOT_COL_1(pos) && pos + 7 < 64 &&
        (board[pos + 7] == 0 || board[pos + 7] < blackPawn)) {
      if (!checkForChecks(pos + 7, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 7));
      }
    }
    // Bottom Right
    if (NOT_COL_8(pos) && pos + 9 < 64 &&
        (board[pos + 9] == 0 || board[pos + 9] < blackPawn)) {
      if (!checkForChecks(pos + 9, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 9));
      }
    }
    // Bottom
    if (pos + 8 < 64 && (board[pos + 8] == 0 || board[pos + 8] < blackPawn)) {
      if (!checkForChecks(pos + 8, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 8));
      }
    }
    // Right
    if (NOT_COL_8(pos) && (board[pos + 1] == 0 || board[pos + 1] < blackPawn)) {
      if (!checkForChecks(pos + 1, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos + 1));
      }
    }
    // Left
    if (NOT_COL_1(pos) && (board[pos - 1] == 0 || board[pos - 1] < blackPawn)) {
      if (!checkForChecks(pos - 1, turn)) {
        this->moves.push_back(std::make_tuple(pos, pos - 1));
      }
    }
    // Castling
    if (!bKingMoved) {
      if (!this->queensideBRookMoved && this->board[1] == Empty &&
          this->board[2] == Empty && this->board[3] == Empty &&
          !checkForChecks(1, turn) && !checkForChecks(2, turn) &&
          !checkForChecks(3, turn) && !checkForChecks(4, turn)) {
        this->moves.push_back(std::make_tuple(4, 2));
      }
      if (!this->kingsideBRookMoved && this->board[5] == Empty &&
          this->board[6] == Empty && !checkForChecks(4, turn) &&
          !checkForChecks(5, turn) && !checkForChecks(6, turn)) {
        this->moves.push_back(std::make_tuple(4, 6));
      }
    }
  }
  
  // Put the king back
  board[pos] = kingRemoved;
}

std::vector<std::tuple<int, int>> Board::genAllValidMoves(char turn) {
  // When a fen string is passed back to the engine, the previous moves are not recognied therefore the king
  // positions have to be found again. We only do this once per move, however, because as the engine is looking
  // ahead the king position will be updated everytime makeMove is called
  if (bKingPos == -1 || wKingPos == -1) {
    for (int i = 0; i < 64; i++) {
      if (board[i] == whiteKing) {
        wKingPos = i;
      } else if (board[i] == blackKing) {
        bKingPos = i;
      }
    }
  }

  // TODO: Should this be called?
  moves.clear();
  // Iterate through board, calling Moves on each piece that is yours
  // except king
  if (this->turn == 'w') {
    for (int i = 0; i < 64; i++) {
      if (this->board[i] > 0 && this->board[i] < 6) {
        this->genValidMoves(i, this->turn);
      }
    }
  } else if (this->turn == 'b') {
    for (int i = 0; i < 64; i++) {
      if (this->board[i] > 6 && this->board[i] < 12) {
        this->genValidMoves(i, this->turn);
      }
    }
  }

  // If king is in check after move, erase move from moves vector
  for (int i = 0; i < this->moves.size(); i++) {
    int pos = std::get<0>(this->moves[i]);
    int i2 = std::get<1>(this->moves[i]);
    int temp = this->board[i2];
    // Special case for en Passant
    // bool enPassantMoveMade = false;
    // if ((board[pos] == whitePawn || board[pos] == blackPawn) &&
    //     (abs(i2 - pos) == 7 || abs(i2 - pos) == 9) && board[i2] == 0) {
    //   enPassantMoveMade = true;
    //   this->board[std::get<1>(enPassant)] = 0;
    // }
    this->board[i2] = this->board[pos];
    this->board[pos] = 0;
    bool inCheck =
        this->checkForChecks(turn == 'w' ? wKingPos : bKingPos, turn);
    // Reset pieces
    // if (enPassantMoveMade) {
    //   this->board[std::get<1>(enPassant)] = turn == 'w' ? blackPawn : whitePawn;
    // }
    this->board[pos] = this->board[i2];
    this->board[i2] = temp;
    if (inCheck) {
      this->moves.erase(this->moves.begin() + i);
      i--;
    }
  }

  // Add king moves into moves vector
  int kingPos = turn == 'w' ? wKingPos : bKingPos;
  this->genValidMoves(kingPos, turn);
  return moves;
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
    default:
      std::cout << "Error: no piece at input location" << std::endl;
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
      std::cout << "Error: no piece at input location" << std::endl;
      break;
    }
  }
}

void Board::clearBoard() {}