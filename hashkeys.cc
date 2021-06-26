#include <cstdint>

#include "defs.h"

std::uint64_t GeneratePosKey(const S_BOARD& pos) {
  std::uint64_t finalKey = 0;
  for (int sq = 0; sq < BRD_SQ_NUM; ++sq) {
    int piece = pos.pieces[sq];
    if (piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
      assert(piece >= wP && piece <= bK);
      finalKey ^= PieceKeys[piece][sq];
    }
  }

  if (pos.side == WHITE) {
    finalKey ^= SideKey;
  }

  if (pos.enPas != NO_SQ) {
    assert(pos.enPas >= 0 && pos.enPas < BRD_SQ_NUM);
    finalKey ^= PieceKeys[EMPTY][pos.enPas];
  }

  assert(pos.castlePerm >= 0 && pos.castlePerm <= 15);

  finalKey ^= CastleKeys[pos.castlePerm];

  return finalKey;
}