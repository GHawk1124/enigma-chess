#include "mini_max.h"

int MiniMax::max(int a, int b) { return a > b ? a : b; }

int MiniMax::min(int a, int b) { return a < b ? a : b; }

int MiniMax::evaluate(std::array<unsigned int, 64> board_position) {
  this->score = 0;
  int score_player_flip = this->player == 'w' ? 1 : -1;
  for (int i = 0; i < board_position.size(); i++) {
    if (board_position[i] == 0) {
      continue;
    }
    if (board_position[i] == 1) {
      score += 1;
    }
    if (board_position[i] == 2) {
      score += 3;
    }
    if (board_position[i] == 3) {
      score += 3;
    }
    if (board_position[i] == 4) {
      score += 5;
    }
    if (board_position[i] == 5) {
      score += 9;
    }
    if (board_position[i] == 7) {
      score -= 1;
    }
    if (board_position[i] == 8) {
      score -= 3;
    }
    if (board_position[i] == 9) {
      score -= 3;
    }
    if (board_position[i] == 10) {
      score -= 5;
    }
    if (board_position[i] == 11) {
      score -= 9;
    }
  }
  return score_player_flip * score;
}

// Create a vector of vectors... etc of dimension equal to the depth value
// that stores the best move for each depth
std::string MiniMax::minimax() {
  std::vector<std::vector<std::string>> best_moves;
  for (int i = 0; i < this->depth; i++) {
    best_moves.push_back(std::vector<std::string>());
  }
  this->board.genAllValidMoves(this->player);
  for (int i = 0; i < this->board.moves.size(); i++) {
    this->board.makeMove(std::get<0>(this->board.moves[i]),
                         std::get<1>(this->board.moves[i]));
    this->score = this->evaluate(this->board.getBoard());
    this->board.clearBoard();
    best_moves[0].push_back(std::to_string(std::get<0>(this->board.moves[i])) +
                            std::to_string(std::get<1>(this->board.moves[i])));
  }
  for (int i = 1; i < this->depth; i++) {
    for (int j = 0; j < best_moves[i - 1].size(); j++) {
      this->board.genAllValidMoves(this->player);
      for (int k = 0; k < this->board.moves.size(); k++) {
        this->board.makeMove(std::get<0>(this->board.moves[k]),
                             std::get<1>(this->board.moves[k]));
        this->score = this->evaluate(this->board.getBoard());
        this->board.clearBoard();
        best_moves[i].push_back(best_moves[i - 1][j] +
                                std::to_string(std::get<0>(this->board.moves[k])) +
                                std::to_string(std::get<1>(this->board.moves[k])));
      }
    }
  }
  return best_moves[this->depth - 1][0];
}