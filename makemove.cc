#include <cstdint>
#include <iostream>

#include "defs.h"

void HASH_PCE(S_BOARD& pos, int pce, int sq) {
  pos.posKey ^= PieceKeys[pce][sq];
}

void HASH_CA(S_BOARD& pos) { pos.posKey ^= CastleKeys[pos.castlePerm]; }

void HASH_SIDE(S_BOARD& pos) { pos.posKey ^= SideKey; }

void HASH_EP(S_BOARD& pos) { pos.posKey ^= PieceKeys[EMPTY][pos.enPas]; }

// Castle permissions can be updated after a move is made like this:
// pos.castlePerm &= CastlePerm[from]
const std::array<int, 120> CastlePerm{
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  //
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  //
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,  //
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  //
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  //
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  //
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  //
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  //
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  //
    15, 7,  15, 15, 15, 3,  15, 15, 11, 15,  //
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  //
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15   //
};

namespace {
void ClearPiece(const int sq, S_BOARD& pos) {
  assert(SqOnBoard(sq));

  const int pce = pos.pieces[sq];

  assert(PieceValid(pce));

  const int col = PieceCol[pce];

  HASH_PCE(pos, pce, sq);

  pos.pieces[sq] = EMPTY;
  pos.material[col] -= PieceVal[pce];

  if (PieceBig[pce]) {
    --pos.bigPce[col];
    if (PieceMaj[pce]) {
      --pos.majPce[col];
    } else {
      assert(PieceMin[pce]);
      --pos.minPce[col];
    }
  } else {
    CLRBIT(pos.pawns[col], SQ64(sq));
    CLRBIT(pos.pawns[BOTH], SQ64(sq));
  }

  int t_pceNum = -1;
  for (int index = 0; index < pos.pceNum[pce]; ++index) {
    if (pos.pList[pce][index] == sq) {
      // The piece on sq is the t_pceNum-th piece of that type on the board. We
      // need this to update the pList array.
      t_pceNum = index;
      break;
    }
  }
  assert(t_pceNum != -1);
  --pos.pceNum[pce];
  // I don't understand this.
  pos.pList[pce][t_pceNum] = pos.pList[pce][pos.pceNum[pce]];
}

void AddPiece(const int sq, S_BOARD& pos, const int pce) {
  assert(PieceValid(pce));
  assert(SqOnBoard(sq));

  int col = PieceCol[pce];

  HASH_PCE(pos, pce, sq);

  pos.pieces[sq] = pce;

  if (PieceBig[pce]) {
    pos.bigPce[col]++;
    if (PieceMaj[pce]) {
      pos.majPce[col]++;
    } else {
      pos.minPce[col]++;
    }
  } else {
    SETBIT(pos.pawns[col], SQ64(sq));
    SETBIT(pos.pawns[BOTH], SQ64(sq));
  }

  pos.material[col] += PieceVal[pce];
  pos.pList[pce][pos.pceNum[pce]] = sq;
  ++pos.pceNum[pce];
}

void MovePiece(const int from, const int to, S_BOARD& pos) {
  assert(SqOnBoard(from));
  assert(SqOnBoard(to));

  int index = 0;
  int pce = pos.pieces[from];
  int col = PieceCol[pce];

  int t_PieceNum = false;

  HASH_PCE(pos, pce, from);
  pos.pieces[from] = EMPTY;

  HASH_PCE(pos, pce, to);
  pos.pieces[to] = pce;

  if (!PieceBig[pce]) {
    CLRBIT(pos.pawns[col], SQ64(from));
    CLRBIT(pos.pawns[BOTH], SQ64(from));
    SETBIT(pos.pawns[col], SQ64(to));
    SETBIT(pos.pawns[BOTH], SQ64(to));
  }

  for (index = 0; index < pos.pceNum[pce]; ++index) {
    if (pos.pList[pce][index] == from) {
      pos.pList[pce][index] = to;
      t_PieceNum = true;
      break;
    }
  }
  assert(t_PieceNum);
}

}  // namespace

bool MakeMove(S_BOARD& pos, int move) {
  assert(CheckBoard(pos));

  int from = FROMSQ(move);
  int to = TOSQ(move);
  int side = pos.side;

  assert(SqOnBoard(from));
  assert(SqOnBoard(to));
  assert(SideValid(side));
  assert(PieceValid(pos.pieces[from]));

  pos.history[pos.hisPly].posKey = pos.posKey;

  if (move & MFLAGEP) {
    if (side == WHITE) {
      ClearPiece(to - 10, pos);
    } else {
      ClearPiece(to + 10, pos);
    }
  } else if (move & MFLAGCA) {
    switch (to) {
      case C1:
        MovePiece(A1, D1, pos);
        break;
      case C8:
        MovePiece(A8, D8, pos);
        break;
      case G1:
        MovePiece(H1, F1, pos);
        break;
      case G8:
        MovePiece(H8, F8, pos);
        break;
      default:
        assert(false);
        break;
    }
  }

  if (pos.enPas != NO_SQ) {
    HASH_EP(pos);
  }
  HASH_CA(pos);

  pos.history[pos.hisPly].move = move;
  pos.history[pos.hisPly].fiftyMove = pos.fiftyMove;
  pos.history[pos.hisPly].enPas = pos.enPas;
  pos.history[pos.hisPly].castlePerm = pos.castlePerm;

  pos.castlePerm &= CastlePerm[from];
  pos.castlePerm &= CastlePerm[to];
  pos.enPas = NO_SQ;

  HASH_CA(pos);

  int captured = CAPTURED(move);
  pos.fiftyMove++;

  if (captured != EMPTY) {
    assert(PieceValid(captured));
    ClearPiece(to, pos);
    pos.fiftyMove = 0;
  }

  ++pos.hisPly;
  ++pos.ply;

  if (PiecePawn[pos.pieces[from]]) {
    pos.fiftyMove = 0;
    if (move & MFLAGPS) {
      if (side == WHITE) {
        pos.enPas = from + 10;
        assert(RanksBrd[pos.enPas] == RANK_3);
      } else {
        pos.enPas = from - 10;
        assert(RanksBrd[pos.enPas] == RANK_6);
      }
      HASH_EP(pos);
    }
  }

  MovePiece(from, to, pos);

  int prPce = PROMOTED(move);
  if (prPce != EMPTY) {
    assert(PieceValid(prPce) && !PiecePawn[prPce]);
    ClearPiece(to, pos);
    AddPiece(to, pos, prPce);
  }

  if (PieceKing[pos.pieces[to]]) {
    pos.KingSq[pos.side] = to;
  }

  pos.side ^= 1;
  HASH_SIDE(pos);

  if (SqAttacked(pos.KingSq[side], pos.side, pos)) {
    TakeMove(pos);
    return false;
  }

  return true;
}

void TakeMove(S_BOARD& pos) {
  assert(CheckBoard(pos));

  --pos.hisPly;
  --pos.ply;

  int move = pos.history[pos.hisPly].move;
  int from = FROMSQ(move);
  int to = TOSQ(move);

  assert(SqOnBoard(from));
  assert(SqOnBoard(to));

  if (pos.enPas != NO_SQ) HASH_EP(pos);
  HASH_CA(pos);

  pos.castlePerm = pos.history[pos.hisPly].castlePerm;
  pos.fiftyMove = pos.history[pos.hisPly].fiftyMove;
  pos.enPas = pos.history[pos.hisPly].enPas;

  if (pos.enPas != NO_SQ) HASH_EP(pos);
  HASH_CA(pos);

  pos.side ^= 1;
  HASH_SIDE(pos);

  if (MFLAGEP & move) {
    if (pos.side == WHITE) {
      AddPiece(to - 10, pos, bP);
    } else {
      AddPiece(to + 10, pos, wP);
    }
  } else if (MFLAGCA & move) {
    switch (to) {
      case C1:
        MovePiece(D1, A1, pos);
        break;
      case C8:
        MovePiece(D8, A8, pos);
        break;
      case G1:
        MovePiece(F1, H1, pos);
        break;
      case G8:
        MovePiece(F8, H8, pos);
        break;
      default:
        assert(false);
        break;
    }
  }

  MovePiece(to, from, pos);

  if (PieceKing[pos.pieces[from]]) {
    pos.KingSq[pos.side] = from;
  }

  int captured = CAPTURED(move);
  if (captured != EMPTY) {
    assert(PieceValid(captured));
    AddPiece(to, pos, captured);
  }

  if (PROMOTED(move) != EMPTY) {
    assert(PieceValid(PROMOTED(move)) && !PiecePawn[PROMOTED(move)]);
    ClearPiece(from, pos);
    AddPiece(from, pos, (PieceCol[PROMOTED(move)] == WHITE ? wP : bP));
  }

  assert(CheckBoard(pos));
}