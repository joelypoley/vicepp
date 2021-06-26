#include <iostream>

#include "defs.h"

std::uint32_t FROMSQ(std::uint32_t m) { return m & 0x7FUL; };
std::uint32_t TOSQ(std::uint32_t m) { return (m >> 7) & 0x7FUL; };
std::uint32_t CAPTURED(std::uint32_t m) { return (m >> 14) & 0xFUL; };
std::uint32_t PROMOTED(std::uint32_t m) { return (m >> 20) & 0xFUL; };

std::uint32_t MOVE(std::uint32_t f, std::uint32_t t, std::uint32_t ca,
                   std::uint32_t pro, std::uint32_t fl) {
  return f | (t << 7) | (ca << 14) | (pro << 20) | fl;
}

bool SQOFFBOARD(int sq) { return FilesBrd.at(sq) == OFFBOARD; }

std::array<int, 8> LoopSlidePce = {wB, wR, wQ, 0, bB, bR, bQ, 0};
std::array<int, 6> LoopNonSlidePce = {wN, wK, 0, bN, bK, 0};

std::array<int, 2> LoopSlideIndex = {0, 4};
std::array<int, 2> LoopNonSlideIndex = {0, 3};

std::array<std::array<int, 9>, 13> PceDir = {
    {{0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0},
     {-8, -19, -21, -12, 8, 19, 21, 12},
     {-9, -11, 11, 9, 0, 0, 0, 0},
     {-1, -10, 1, 10, 0, 0, 0, 0},
     {-1, -10, 1, 10, -9, -11, 11, 9},
     {-1, -10, 1, 10, -9, -11, 11, 9},
     {0, 0, 0, 0, 0, 0, 0},
     {-8, -19, -21, -12, 8, 19, 21, 12},
     {-9, -11, 11, 9, 0, 0, 0, 0},
     {-1, -10, 1, 10, 0, 0, 0, 0},
     {-1, -10, 1, 10, -9, -11, 11, 9},
     {-1, -10, 1, 10, -9, -11, 11, 9}}};

std::array<int, 13> NumDir = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};

void AddQuietMove(const S_BOARD& pos, std::uint32_t move, S_MOVELIST* list) {
  assert(SqOnBoard(FROMSQ(move)));
  assert(SqOnBoard(TOSQ(move)));
  list->moves.push_back({/*move=*/move, /*score=*/0});
}

void AddCaptureMove(const S_BOARD& pos, std::uint32_t move, S_MOVELIST* list) {
  assert(SqOnBoard(FROMSQ(move)));
  assert(SqOnBoard(TOSQ(move)));
  assert(PieceValid(CAPTURED(move)));
  list->moves.push_back({/*move=*/move, /*score=*/0});
}

void AddEnPassantMove(const S_BOARD& pos, std::uint32_t move,
                      S_MOVELIST* list) {
  assert(SqOnBoard(FROMSQ(move)));
  assert(SqOnBoard(TOSQ(move)));
  list->moves.push_back({/*move=*/move, /*score=*/0});
}

void AddWhitePawnCapMove(const S_BOARD& pos, const int from, const int to,
                         const int cap, S_MOVELIST* list) {
  assert(PieceValidEmpty(cap));
  assert(SqOnBoard(from));
  assert(SqOnBoard(to));

  if (RanksBrd.at(from) == RANK_7) {
    AddCaptureMove(pos, MOVE(from, to, cap, wQ, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wR, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wB, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, wN, 0), list);
  } else {
    AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

void AddWhitePawnMove(const S_BOARD& pos, const int from, const int to,
                      S_MOVELIST* list) {
  assert(SqOnBoard(from));
  assert(SqOnBoard(to));
  if (RanksBrd.at(from) == RANK_7) {
    AddQuietMove(pos, MOVE(from, to, EMPTY, wQ, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wR, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wB, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, wN, 0), list);
  } else {
    AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

void AddBlackPawnCapMove(const S_BOARD& pos, const int from, const int to,
                         const int cap, S_MOVELIST* list) {
  assert(PieceValidEmpty(cap));
  assert(SqOnBoard(from));
  assert(SqOnBoard(to));
  if (RanksBrd[from] == RANK_2) {
    AddCaptureMove(pos, MOVE(from, to, cap, bQ, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, bR, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, bB, 0), list);
    AddCaptureMove(pos, MOVE(from, to, cap, bN, 0), list);
  } else {
    AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

void AddBlackPawnMove(const S_BOARD& pos, const int from, const int to,
                      S_MOVELIST* list) {
  assert(SqOnBoard(from));
  assert(SqOnBoard(to));
  if (RanksBrd[from] == RANK_2) {
    AddQuietMove(pos, MOVE(from, to, EMPTY, bQ, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, bR, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, bB, 0), list);
    AddQuietMove(pos, MOVE(from, to, EMPTY, bN, 0), list);
  } else {
    AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

void GenerateAllMoves(const S_BOARD& pos, S_MOVELIST* list) {
  list->moves.clear();

  int side = pos.side;

  std::cout << "\n\nSide: " << side << '\n';

  if (side == WHITE) {
    for (int pceNum = 0; pceNum < pos.pceNum.at(wP); ++pceNum) {
      int sq = pos.pList.at(wP).at(pceNum);
      assert(SqOnBoard(sq));

      if (pos.pieces.at(sq + 10) == EMPTY) {
        AddWhitePawnMove(pos, sq, sq + 10, list);
        if (RanksBrd.at(sq) == RANK_2 && pos.pieces.at(sq + 20) == EMPTY) {
          AddQuietMove(pos, MOVE(sq, sq + 20, EMPTY, EMPTY, MFLAGPS), list);
        }
      }

      assert(CheckBoard(pos));
      if (!SQOFFBOARD(sq + 9) && PieceCol.at(pos.pieces.at(sq + 9)) == BLACK) {
        AddWhitePawnCapMove(pos, sq, sq + 9, pos.pieces.at(sq + 9), list);
      }

      if (!SQOFFBOARD(sq + 11) &&
          PieceCol.at(pos.pieces.at(sq + 11)) == BLACK) {
        AddWhitePawnCapMove(pos, sq, sq + 11, pos.pieces.at(sq + 11), list);
      }

      if (pos.enPas != NO_SQ) {
        if (sq + 9 == pos.enPas) {
          AddEnPassantMove(pos, MOVE(sq, sq + 9, EMPTY, EMPTY, MFLAGEP), list);
        }
        if (sq + 11 == pos.enPas) {
          AddEnPassantMove(pos, MOVE(sq, sq + 11, EMPTY, EMPTY, MFLAGEP), list);
        }
      }
    }
  } else {
    for (int pceNum = 0; pceNum < pos.pceNum[bP]; ++pceNum) {
      int sq = pos.pList[bP][pceNum];
      assert(SqOnBoard(sq));

      if (pos.pieces[sq - 10] == EMPTY) {
        AddBlackPawnMove(pos, sq, sq - 10, list);
        if (RanksBrd[sq] == RANK_7 && pos.pieces[sq - 20] == EMPTY) {
          AddQuietMove(pos, MOVE(sq, (sq - 20), EMPTY, EMPTY, MFLAGPS), list);
        }
      }

      if (!SQOFFBOARD(sq - 9) && PieceCol[pos.pieces[sq - 9]] == WHITE) {
        AddBlackPawnCapMove(pos, sq, sq - 9, pos.pieces[sq - 9], list);
      }

      if (!SQOFFBOARD(sq - 11) && PieceCol[pos.pieces[sq - 11]] == WHITE) {
        AddBlackPawnCapMove(pos, sq, sq - 11, pos.pieces[sq - 11], list);
      }

      if (pos.enPas != NO_SQ) {
        if (sq - 9 == pos.enPas) {
          AddEnPassantMove(pos, MOVE(sq, sq - 9, EMPTY, EMPTY, MFLAGEP), list);
        }
        if (sq - 11 == pos.enPas) {
          AddEnPassantMove(pos, MOVE(sq, sq - 11, EMPTY, EMPTY, MFLAGEP), list);
        }
      }
    }
  }

  // Loop for slide pieces.
  int pceIndex = LoopSlideIndex[side];
  int pce = LoopSlidePce[pceIndex];
  ++pceIndex;
  while (pce != 0) {
    assert(PieceValid(pce));
    std::cout << "sliders pceIndex: " << pceIndex << " pce: " << pce << '\n';

    pce = LoopSlidePce[pceIndex];
    ++pceIndex;
  }

  // Loop for non-slide pieces.
  pceIndex = LoopNonSlideIndex[side];
  pce = LoopNonSlidePce[pceIndex];
  ++pceIndex;

  while (pce != 0) {
    assert(PieceValid(pce));
    std::cout << "non slides pceIndex: " << pceIndex << " pce: " << pce
              << std::endl;

    for (int pceNum = 0; pceNum < pos.pceNum[pce]; ++pceNum) {
      int sq = pos.pList[pce][pceNum];
      assert(SqOnBoard(sq));
      std::cout << "Piece: " << PceChar[pce] << " on " << PrSq(sq) << '\n';

      for (int i = 0; i < NumDir[pce]; ++i) {
        int dir = PceDir[pce][i];
        int t_sq = sq + dir;

        if (SQOFFBOARD(t_sq)) {
          continue;
        }

        if (pos.pieces[t_sq] != EMPTY) {
          if (PieceCol[pos.pieces[t_sq]] == (side ^ 1)) {
            std::cout << "\t\tCapture on " << PrSq(t_sq) << '\n';
          }
          continue;
        }

        std::cout << "\t\tNormal on " << PrSq(t_sq) << '\n';
      }
    }
    pce = LoopNonSlidePce[pceIndex];
    ++pceIndex;
  }
}