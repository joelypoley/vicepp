#include "absl/random/random.h"
#include "defs.h"

std::array<int, BRD_SQ_NUM> Sq120ToSq64;
std::array<int, 64> Sq64ToSq120;

std::array<std::uint64_t, 64> SetMask;
std::array<std::uint64_t, 64> ClearMask;

std::array<std::array<std::uint64_t, 120>, 13> PieceKeys;
std::uint64_t SideKey;
std::array<std::uint64_t, 16> CastleKeys;

std::array<int, BRD_SQ_NUM> FilesBrd;
std::array<int, BRD_SQ_NUM> RanksBrd;

void InitFilesRanksBrd() {
  FilesBrd.fill(OFFBOARD);
  RanksBrd.fill(OFFBOARD);

  for (int rank = RANK_1; rank <= RANK_8; ++rank) {
    for (int file = FILE_A; file <= FILE_H; ++file) {
      int sq = FR2SQ(file, rank);
      FilesBrd[sq] = file;
      RanksBrd[sq] = rank;
    }
  }
}

void InitHashKeys() {
  absl::BitGen bitgen;

  for (auto& arr : PieceKeys) {
    for (auto& val : arr) {
      val = absl::Uniform<std::uint64_t>(bitgen);
    }
  }
  SideKey = absl::Uniform<std::uint64_t>(bitgen);
  for (auto& val : CastleKeys) {
    val = absl::Uniform<std::uint64_t>(bitgen);
  }
}

void InitBitMasks() {
  SetMask.fill(0);
  ClearMask.fill(0);

  for (int i = 0; i < 64; ++i) {
    SetMask[i] |= (1ULL << i);
    ClearMask[i] = ~SetMask[i];
  }
}

void InitSq120To64() {
  Sq120ToSq64.fill(65);
  Sq64ToSq120.fill(120);

  int sq64 = 0;
  for (int rank = RANK_1; rank <= RANK_8; ++rank) {
    for (int file = FILE_A; file <= FILE_H; ++file) {
      int sq = FR2SQ(file, rank);
      Sq64ToSq120[sq64] = sq;
      Sq120ToSq64[sq] = sq64;
      ++sq64;
    }
  }
}

int SQ64(int sq120) { return Sq120ToSq64[sq120]; }
int SQ120(int sq64) { return Sq64ToSq120[sq64]; }

int FR2SQ(int f, int r) { return 21 + f + r * 10; }

std::uint64_t CLRBIT(std::uint64_t& bb, std::uint64_t sq) {
  return bb &= ClearMask[sq];
}

std::uint64_t SETBIT(std::uint64_t& bb, std::uint64_t sq) {
  return bb |= SetMask[sq];
}

void AllInit() {
  InitSq120To64();
  InitBitMasks();
  InitHashKeys();
  InitFilesRanksBrd();
}