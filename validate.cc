#include "defs.h"

bool SqOnBoard(const int sq) { return FilesBrd[sq] == OFFBOARD ? false : true; }

bool SideValid(const int side) {
  return (side == WHITE || side == BLACK) ? true : false;
}

bool FileRankValid(const int fr) { return (fr >= 0 && fr <= 7) ? true : false; }

bool PieceValidEmpty(const int pce) {
  return (pce >= EMPTY && pce <= bK) ? true : false;
}

bool PieceValid(const int pce) {
  return (pce >= wP && pce <= bK) ? true : false;
}