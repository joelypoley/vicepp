#include <fmt/core.h>

#include <iostream>

#include "absl/strings/str_cat.h"
#include "defs.h"

#define CASTLE1 "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
#define CASTLE2 \
  "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main() {
  fmt::print("-----------------------------------------\n");
  AllInit();

  S_BOARD board = {};

  assert(ParseFen(CASTLE2, board) != -1);
  PrintBoard(board);

  S_MOVELIST list = {};

  GenerateAllMoves(board, &list);

  PrintMoveList(list);

  fmt::print("Hello, world!\n");

  std::cout << absl::StrCat("\nHello, ", "World!\n");
}