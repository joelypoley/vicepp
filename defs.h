#ifndef DEFS_H
#define DEFS_H

#include <array>
#include <cstdint>
#include <vector>

#include "absl/strings/string_view.h"

constexpr absl::string_view NAME = "Vice 1.0";
constexpr int BRD_SQ_NUM = 120;
constexpr int MAXGAMEMOVES = 2048;
constexpr int MAXPOSITIONMOVES = 256;

constexpr absl::string_view START_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };
enum {
  FILE_A,
  FILE_B,
  FILE_C,
  FILE_D,
  FILE_E,
  FILE_F,
  FILE_G,
  FILE_H,
  FILE_NONE
};
enum {
  RANK_1,
  RANK_2,
  RANK_3,
  RANK_4,
  RANK_5,
  RANK_6,
  RANK_7,
  RANK_8,
  RANK_NONE
};
enum { WHITE, BLACK, BOTH };

enum {
  A1 = 21,
  B1,
  C1,
  D1,
  E1,
  F1,
  G1,
  H1,
  A2 = 31,
  B2,
  C2,
  D2,
  E2,
  F2,
  G2,
  H2,
  A3 = 41,
  B3,
  C3,
  D3,
  E3,
  F3,
  G3,
  H3,
  A4 = 51,
  B4,
  C4,
  D4,
  E4,
  F4,
  G4,
  H4,
  A5 = 61,
  B5,
  C5,
  D5,
  E5,
  F5,
  G5,
  H5,
  A6 = 71,
  B6,
  C6,
  D6,
  E6,
  F6,
  G6,
  H6,
  A7 = 81,
  B7,
  C7,
  D7,
  E7,
  F7,
  G7,
  H7,
  A8 = 91,
  B8,
  C8,
  D8,
  E8,
  F8,
  G8,
  H8,
  NO_SQ,
  OFFBOARD,
};

// Castling rights.
enum {
  WKCA = 1,
  WQCA = 2,
  BKCA = 4,
  BQCA = 8,
};

struct S_MOVE {
  // `move` stores different information in its bits (where 0 is the least
  // siginificant bit):
  //    [0..6] - From square (120 based index).
  //    [7..13] - To square (120 based index).
  //    [14..17] - Captured.
  //    [18] - En passant.
  //    [19] - Pawn start.
  //    [20..24] - Promoted piece.
  //    [25] - Castle.
  std::uint32_t move;
  int score;
};

struct S_MOVELIST {
  // In the real vice engine this is a c-style array and a count of the number
  // of elements in the array (a "roll-your-own" vector). I'm just wrapping a
  // vector here so it's easy for me to follow along with the video series.
  std::vector<S_MOVE> moves;
};

struct S_UNDO {
  int move;
  int castlePerm;
  int enPas;
  int fiftyMove;
  std::uint64_t posKey;
};

struct S_BOARD {
  // The values in this array come from the piece enum.
  std::array<int, BRD_SQ_NUM> pieces;

  // Bitboard of pawns. Indexed by the pawns enum.
  std::array<std::uint64_t, 3> pawns;

  std::array<int, 2> KingSq;

  // Color enum stored here.
  int side;
  // Square enum stored here.
  int enPas;
  int fiftyMove;

  // How many 1/2 moves in the current search.
  int ply;
  // How many 1/2 moves in the game + the current search.
  int hisPly;

  // Castling rights enum stored here.
  int castlePerm;

  // Hash of the position.
  std::uint64_t posKey;

  // The number of pieces on the board. Indexed by piece type. The first index
  // in the array, which corresponds to the EMPTY piece is undefined (I think
  // it's always zero? It's definitely not the number of empty squares.).
  std::array<int, 13> pceNum;

  // Number of pieces (excluding pawns) indexed by color.
  std::array<int, 3> bigPce;
  // Number of major pieces indexed by color.
  std::array<int, 3> majPce;
  // Number of minor pieces indexed by color.
  std::array<int, 3> minPce;
  // Material score indexed by color.
  std::array<int, 2> material;

  std::array<S_UNDO, MAXGAMEMOVES> history;

  // Gives the list of the square the nth piece of that piecetype is on. For
  // example, pList[wP][3] is the square of the 4th white pawn.
  std::array<std::array<int, 10>, 13> pList;
};

// GLOBALS

extern std::array<int, BRD_SQ_NUM> Sq120ToSq64;
extern std::array<int, 64> Sq64ToSq120;
extern std::array<std::uint64_t, 64> SetMask;
extern std::array<std::uint64_t, 64> ClearMask;
extern std::array<std::array<std::uint64_t, 120>, 13> PieceKeys;
extern std::uint64_t SideKey;
extern std::array<std::uint64_t, 16> CastleKeys;
constexpr absl::string_view PceChar = ".PNBRQKpnbrqk";
constexpr absl::string_view SideChar = "wb-";
constexpr absl::string_view RankChar = "12345678";
constexpr absl::string_view FileChar = "abcdefgh";

extern std::array<int, 13> PieceBig;
extern std::array<int, 13> PieceMaj;
extern std::array<int, 13> PieceMin;
extern std::array<int, 13> PieceVal;
extern std::array<int, 13> PieceCol;

extern std::array<int, BRD_SQ_NUM> FilesBrd;
extern std::array<int, BRD_SQ_NUM> RanksBrd;

extern std::array<int, 13> PieceKnight;
extern std::array<int, 13> PieceKing;
extern std::array<int, 13> PieceRookQueen;
extern std::array<int, 13> PieceBishopQueen;
extern std::array<bool, 13> PieceSlides;

// GAME MOVE
std::uint32_t FROMSQ(std::uint32_t m);
std::uint32_t TOSQ(std::uint32_t m);
std::uint32_t CAPTURED(std::uint32_t m);
std::uint32_t PROMOTED(std::uint32_t m);

constexpr std::uint32_t MFLAGEP = 0x40000UL;
constexpr std::uint32_t MFLAGPS = 0x80000UL;
constexpr std::uint32_t MFLAGCA = 0x1000000UL;
constexpr std::uint32_t MFLAGCAP = 0x7C000UL;
constexpr std::uint32_t MFLAGPROM = 0xF00000UL;

// "MACROS"
int FR2SQ(int f, int r);
int SQ64(int sq120);
int SQ120(int sq64);
int POP(std::uint64_t& bb);
int CNT(std::uint64_t b);
std::uint64_t CLRBIT(std::uint64_t& bb, std::uint64_t sq);
std::uint64_t SETBIT(std::uint64_t& bb, std::uint64_t sq);

bool IsBQ(int p);
bool IsRQ(int p);
bool IsKn(int p);
bool IsKi(int p);

// FUNCTIONS

// init.cc
void AllInit();

// bitboards.cc
void PrintBitBoard(std::uint64_t bb);
int PopBit(std::uint64_t& bb);
int CountBits(std::uint64_t b);

// hashkeys.cc
std::uint64_t GeneratePosKey(const S_BOARD& pos);

// board.cc
void ResetBoard(S_BOARD& pos);
void PrintBoard(const S_BOARD& pos);
int ParseFen(absl::string_view fen, S_BOARD& pos);
void UpdateListsMaterial(S_BOARD& pos);
bool CheckBoard(const S_BOARD& pos);

// attack.cc

bool SqAttacked(const int sq, const int side, const S_BOARD& pos);

// io.cc
std::string PrSq(const int sq);
std::string PrMove(const int move);
void PrintMoveList(const S_MOVELIST& list);

// validate.cc
bool SqOnBoard(const int sq);
bool SideValid(const int side);
bool FileRankValid(const int fr);
bool PieceValidEmpty(const int pce);
bool PieceValid(const int pce);

// movegen.cc
void GenerateAllMoves(const S_BOARD& pos, S_MOVELIST* list);

#endif