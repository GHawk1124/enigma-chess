#ifndef MINI_MAX_H
#define MINI_MAX_H

#include "board.h"

class MiniMax {
  private:
    Board board;
    int depth = 2;
    int bestMove;
    int bestScore;
    int alpha;
    int beta;
    int score;
    char player = 'w';
    int max(int a, int b);
    int min(int a, int b);
    int evaluate(std::array<unsigned int, 64> board_position);
    std::string minimax();
};

#endif