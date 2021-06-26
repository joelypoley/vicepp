#include <algorithm>
#include <iostream>

#include "absl/strings/string_view.h"
#include "defs.h"

bool CheckBoard(const S_BOARD& pos) {
  S_BOARD t_pos = {};

  t_pos.pawns[WHITE] = pos.pawns[WHITE];
  t_pos.pawns[BLACK] = pos.pawns[BLACK];
  t_pos.pawns[BOTH] = pos.pawns[BOTH];
  for (int t_piece = wP; t_piece <= bK; ++t_piece) {
    for (int t_pce_num = 0; t_pce_num < pos.pceNum[t_piece]; ++t_pce_num) {
      int sq120 = pos.pList[t_piece][t_pce_num];
      assert(pos.pieces[sq120] == t_piece);
    }
  }

  for (int sq64 = 0; sq64 < 64; ++sq64) {
    int sq120 = SQ120(sq64);
    int t_piece = pos.pieces[sq120];
    ++t_pos.pceNum[t_piece];
    int color = PieceCol[t_piece];
    if (PieceBig[t_piece]) ++t_pos.bigPce[color];
    if (PieceMin[t_piece]) ++t_pos.minPce[color];
    if (PieceMaj[t_piece]) ++t_pos.majPce[color];

    t_pos.material[color] += PieceVal[t_piece];
  }

  // We don't check the first index is equal in both because the first value in
  // the array corresponds to the EMPTY piece.
  assert(std::equal(t_pos.pceNum.begin() + 1, t_pos.pceNum.end(),
                    pos.pceNum.begin() + 1));

  assert(CNT(t_pos.pawns[WHITE]) == t_pos.pceNum[wP]);
  assert(CNT(t_pos.pawns[BLACK]) == t_pos.pceNum[bP]);
  assert(CNT(t_pos.pawns[BOTH]) == (t_pos.pceNum[wP] + t_pos.pceNum[bP]));

  while (t_pos.pawns[WHITE]) {
    int sq64 = POP(t_pos.pawns[WHITE]);
    assert(pos.pieces[SQ120(sq64)] == wP);
  }

  while (t_pos.pawns[BLACK]) {
    int sq64 = POP(t_pos.pawns[BLACK]);
    assert((pos.pieces[SQ120(sq64)] == bP));
  }

  while (t_pos.pawns[BOTH]) {
    int sq64 = POP(t_pos.pawns[BOTH]);
    assert((pos.pieces[SQ120(sq64)] == bP) || (pos.pieces[SQ120(sq64)] == wP));
  }

  assert(t_pos.material == pos.material);
  assert(t_pos.minPce == pos.minPce);
  assert(t_pos.majPce == pos.majPce);
  assert(t_pos.bigPce == pos.bigPce);

  assert(pos.side == WHITE || pos.side == BLACK);
  assert(GeneratePosKey(pos) == pos.posKey);
  assert(pos.enPas == NO_SQ ||
         (RanksBrd[pos.enPas] == RANK_6 && pos.side == WHITE) ||
         (RanksBrd[pos.enPas] == RANK_3 && pos.side == BLACK));
  assert(pos.pieces[pos.KingSq[WHITE]] == wK);
  assert(pos.pieces[pos.KingSq[BLACK]] == bK);

  return true;
}

void UpdateListsMaterial(S_BOARD& pos) {
  pos.material.fill(0);
  for (int sq = 0; sq < BRD_SQ_NUM; ++sq) {
    int piece = pos.pieces[sq];
    if (piece != OFFBOARD && piece != EMPTY) {
      int color = PieceCol[piece];

      if (PieceBig[piece]) ++pos.bigPce[color];
      if (PieceMin[piece]) ++pos.minPce[color];
      if (PieceMaj[piece]) ++pos.majPce[color];

      pos.material[color] += PieceVal[piece];
      // I don't understand what this is doing. Mapping (piece, number of
      // pieces) -> the first square which has cumulatively has that number of
      // pieces? Also shoudl this be zero initialized at the beginning of this
      // function?
      pos.pList[piece][pos.pceNum[piece]] = sq;
      ++pos.pceNum[piece];

      if (piece == wK) pos.KingSq[WHITE] = sq;
      if (piece == bK) pos.KingSq[BLACK] = sq;

      if (piece == wP) {
        SETBIT(pos.pawns[WHITE], SQ64(sq));
        SETBIT(pos.pawns[BOTH], SQ64(sq));
      } else if (piece == bP) {
        SETBIT(pos.pawns[BLACK], SQ64(sq));
        SETBIT(pos.pawns[BOTH], SQ64(sq));
      }
    }
  }
}

int ParseFen(absl::string_view fen, S_BOARD& pos) {
  int rank = RANK_8;
  int file = FILE_A;
  int piece = 0;

  ResetBoard(pos);

  int i = 0;
  while ((rank >= RANK_1) && i < fen.size()) {
    int count = 1;
    switch (fen[i]) {
      case 'p':
        piece = bP;
        break;
      case 'r':
        piece = bR;
        break;
      case 'n':
        piece = bN;
        break;
      case 'b':
        piece = bB;
        break;
      case 'k':
        piece = bK;
        break;
      case 'q':
        piece = bQ;
        break;
      case 'P':
        piece = wP;
        break;
      case 'R':
        piece = wR;
        break;
      case 'N':
        piece = wN;
        break;
      case 'B':
        piece = wB;
        break;
      case 'K':
        piece = wK;
        break;
      case 'Q':
        piece = wQ;
        break;

      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
        piece = EMPTY;
        count = fen[i] - '0';
        break;

      case '/':
      case ' ':
        rank--;
        file = FILE_A;
        ++i;
        continue;

      default:
        std::cout << "FEN error \n";
        return -1;
    }
    for (int j = 0; j < count; ++j) {
      if (piece != EMPTY) {
        int sq64 = rank * 8 + file;
        int sq120 = SQ120(sq64);
        pos.pieces[sq120] = piece;
      }
      ++file;
    }
    ++i;
  }

  assert(fen[i] == 'w' || fen[i] == 'b');

  pos.side = (fen[i] == 'w') ? WHITE : BLACK;
  i += 2;

  for (int j = 0; j < 4; ++j) {
    if (fen[i] == ' ') {
      break;
    }
    switch (fen[i]) {
      case 'K':
        pos.castlePerm |= WKCA;
        break;
      case 'Q':
        pos.castlePerm |= WQCA;
        break;
      case 'k':
        pos.castlePerm |= BKCA;
        break;
      case 'q':
        pos.castlePerm |= BQCA;
        break;
      default:
        break;
    }
    ++i;
  }
  ++i;

  assert(pos.castlePerm >= 0 && pos.castlePerm <= 15);

  if (fen[i] != '-') {
    file = fen[i] - 'a';
    rank = fen[i + 1] - '1';

    assert(file >= FILE_A && file <= FILE_H);
    assert(rank >= RANK_1 && rank <= RANK_8);

    pos.enPas = FR2SQ(file, rank);
  }

  pos.posKey = GeneratePosKey(pos);
  UpdateListsMaterial(pos);

  return 0;
}

void ResetBoard(S_BOARD& pos) {
  pos.pieces.fill(OFFBOARD);
  for (int i = 0; i < 64; ++i) {
    pos.pieces[SQ120(i)] = EMPTY;
  }

  pos.bigPce.fill(0);
  pos.majPce.fill(0);
  pos.minPce.fill(0);
  pos.material.fill(0);
  pos.pawns.fill(0);

  pos.pceNum.fill(0);
  pos.KingSq.fill(NO_SQ);
  pos.side = BOTH;
  pos.enPas = NO_SQ;
  pos.fiftyMove = 0;
  pos.ply = 0;
  pos.hisPly = 0;
  pos.castlePerm = 0;
  pos.posKey = 0;
}

void PrintBoard(const S_BOARD& pos) {
  std::cout << "Game Board: \n\n";

  for (int rank = RANK_8; rank >= RANK_1; --rank) {
    std::cout << rank + 1 << "  ";
    for (int file = FILE_A; file <= FILE_H; ++file) {
      int sq = FR2SQ(file, rank);
      int piece = pos.pieces[sq];
      std::cout << ' ' << PceChar[piece] << ' ';
    }
    std::cout << '\n';
  }

  std::cout << "\n   ";

  for (int file = FILE_A; file <= FILE_H; ++file) {
    std::cout << ' ' << static_cast<char>('a' + file) << ' ';
  }
  std::cout << '\n';
  std::cout << "side: " << SideChar[pos.side] << '\n';
  std::cout << "enPas: " << pos.enPas << '\n';
  std::cout << "castle: " << (pos.castlePerm & WKCA ? 'K' : '-')
            << (pos.castlePerm & WQCA ? 'Q' : '-')
            << (pos.castlePerm & BKCA ? 'k' : '-')
            << (pos.castlePerm & BQCA ? 'q' : '-') << '\n';
  std::cout << "PosKey: " << pos.posKey << '\n';
}