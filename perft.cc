#include <fmt/core.h>

#include "defs.h"

long leafNodes = 0;

void Perft(int depth, S_BOARD &pos) {
  assert(CheckBoard(pos));

  if (depth == 0) {
    leafNodes++;
    return;
  }

  S_MOVELIST list = {};
  GenerateAllMoves(pos, &list);

  int MoveNum = 0;
  for (MoveNum = 0; MoveNum < list.moves.size(); ++MoveNum) {
    if (!MakeMove(pos, list.moves[MoveNum].move)) {
      continue;
    }
    Perft(depth - 1, pos);
    TakeMove(pos);
  }

  return;
}

long PerftTest(int depth, S_BOARD &pos) {
  assert(CheckBoard(pos));

  PrintBoard(pos);
  printf("\nStarting Test To Depth:%d\n", depth);
  leafNodes = 0;

  S_MOVELIST list = {};
  GenerateAllMoves(pos, &list);

  int move;
  int MoveNum = 0;
  for (MoveNum = 0; MoveNum < list.moves.size(); ++MoveNum) {
    move = list.moves[MoveNum].move;
    if (!MakeMove(pos, move)) {
      continue;
    }
    long cumnodes = leafNodes;
    Perft(depth - 1, pos);
    TakeMove(pos);
    long oldnodes = leafNodes - cumnodes;
    fmt::print("move {} : {} : {}\n", MoveNum + 1, PrMove(move), oldnodes);
  }

  fmt::print("\nTest Complete : {} nodes visited\n", leafNodes);

  return leafNodes;
}