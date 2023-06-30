#include "board.h"
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>


// TO DO:
// Draw by Repetition

int main() {
  Board board;
  // board.printBoard();

  auto startTime = std::chrono::steady_clock::now();
  std::cout<< "time started: "  <<std::endl;

  while(1) {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
    if (elapsedTime >= 5) {
      break;
    }
  }
  std::cout<<"Timer ended"<<std::endl;

  std::vector<std::tuple<int, int, int>> validMoves = board.genAllValidMoves(board.turn);
  board.printValidMoves(validMoves);
  validMoves = board.orderMoves(validMoves);
  board.printValidMoves(validMoves);
  
}