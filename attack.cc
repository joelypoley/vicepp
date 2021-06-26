#include <array>
#include <iostream>

#include "absl/algorithm/container.h"
#include "defs.h"

constexpr std::array<int, 8> KnDir = {-8, -19, -21, -12, 8, 19, 21, 12};
constexpr std::array<int, 4> RkDir = {-1, -10, 1, 10};
constexpr std::array<int, 4> BiDir = {-9, -11, 11, 9};
constexpr std::array<int, 8> KiDir = {-1, -10, 1, 10, -9, -11, 11, 9};

bool IsBQ(int p) { return PieceBishopQueen[(p)]; }
bool IsRQ(int p) { return PieceRookQueen[(p)]; }
bool IsKn(int p) { return PieceKnight[(p)]; }
bool IsKi(int p) { return PieceKing[(p)]; }

bool SqAttacked(const int sq, const int side, const S_BOARD& pos) {
  assert(SqOnBoard(sq));
  assert(SideValid(side));
  assert(CheckBoard(pos));

  if (side == WHITE) {
    if (pos.pieces[sq - 11] == wP || pos.pieces[sq - 9] == wP) {
      return true;
    }
  } else {
    if (pos.pieces[sq + 11] == bP || pos.pieces[sq + 9] == bP) {
      return true;
    }
  }

  // Is attacked by knight?
  for (const int kndir : KnDir) {
    int pce = pos.pieces[sq + kndir];
    if (pce != EMPTY && pce != OFFBOARD && IsKn(pce) && PieceCol[pce] == side) {
      return true;
    }
  }

  // Is attacked by a rook? Or attacked by a queen along a rank/file?
  for (const int rkdir : RkDir) {
    int t_sq = sq + rkdir;
    int pce = pos.pieces[t_sq];
    while (pce != OFFBOARD) {
      if (pce != EMPTY) {
        // We've found a piece. Let's check if it is a Rook or queen.
        if (IsRQ(pce) && PieceCol[pce] == side) {
          return true;
        }
        // Some other piece is here, stop searching in this direction.
        break;
      }
      // Move one step further in the same direction.
      t_sq += rkdir;
      pce = pos.pieces[t_sq];
    }
  }

  // Is attacked by a bishop? Or is attacked by a queen along a diagonal?
  for (const int bidir : BiDir) {
    int t_sq = sq + bidir;
    int pce = pos.pieces[t_sq];
    while (pce != OFFBOARD) {
      if (pce != EMPTY) {
        if (IsBQ(pce) && PieceCol[pce] == side) {
          return true;
        }
        break;
      }
      t_sq += bidir;
      pce = pos.pieces[t_sq];
    }
  }

  // Is attacked by a king?
  for (int kidir : KiDir) {
    int pce = pos.pieces[sq + kidir];
    if (pce != EMPTY && pce != OFFBOARD && IsKi(pce) && PieceCol[pce] == side) {
      return true;
    }
  }

  return false;
}