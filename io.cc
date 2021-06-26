#include <iostream>

#include "absl/strings/str_cat.h"
#include "defs.h"
#include "fmt/core.h"

std::string PrSq(const int sq) {
  int file = FilesBrd[sq];
  int rank = RanksBrd[sq];
  std::string res;
  res.push_back('a' + file);
  res.push_back('1' + rank);
  return res;
}

std::string PrMove(const int move) {
  int ff = FilesBrd[FROMSQ(move)];
  int rf = RanksBrd[FROMSQ(move)];
  int ft = FilesBrd[TOSQ(move)];
  int rt = RanksBrd[TOSQ(move)];

  std::string res;
  res.push_back('a' + ff);
  res.push_back('1' + rf);
  res.push_back('a' + ft);
  res.push_back('1' + rt);

  int promoted = PROMOTED(move);
  if (promoted) {
    char pchar = 'q';
    if (IsKn(promoted)) {
      pchar = 'n';
    } else if (IsRQ(promoted) && !IsBQ(promoted)) {
      pchar = 'r';
    } else if (!IsRQ(promoted) && IsBQ(promoted)) {
      pchar = 'b';
    }
    res.push_back(pchar);
  }

  return res;
}

void PrintMoveList(const S_MOVELIST& list) {
  fmt::print("MoveList:\n");

  for (int i = 0; i < list.moves.size(); ++i) {
    fmt::print("Move: {:2} > {} (score: {})\n", i + 1,
               PrMove(list.moves.at(i).move), list.moves.at(i).score);
  }
}