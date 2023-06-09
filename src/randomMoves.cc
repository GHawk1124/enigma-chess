#include "randomMoves.h"
#include "stdio.h"
#include <emscripten/emscripten.h>

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE char *make_random_valid_move(char *fen) {
  Board board;
  auto decoded = board.decode_fen_c(fen);
  board.setBoard(decoded);
  board.genAllValidMoves(board.turn);
  int random_move = rand() % board.moves.size();
  board.makeMove(std::get<0>(board.moves[random_move]),
                 std::get<1>(board.moves[random_move]));
  char *new_board_state = board.encode_fen_c(board.getBoard());
  return new_board_state;
}
