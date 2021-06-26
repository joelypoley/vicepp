#include <fmt/core.h>

#include <iostream>

#include "absl/strings/str_cat.h"
#include "defs.h"

#define PAWNMOVESW \
  "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define PAWNMOVESB \
  "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"
#define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"

int main() {
  fmt::print("-----------------------------------------\n");
  AllInit();

  S_BOARD board = {};

  ParseFen(KNIGHTSKINGS, board);
  PrintBoard(board);

  S_MOVELIST list = {};

  GenerateAllMoves(board, &list);

  // PrintMoveList(list);

  fmt::print("Hello, world!\n");

  std::cout << absl::StrCat("\nHello, ", "World!\n");
}