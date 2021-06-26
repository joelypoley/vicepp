
#include <cstdint>
#include <iostream>

#include "defs.h"

constexpr std::array<int, 64> BitTable = {
    63, 30, 3,  32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34,
    61, 29, 2,  51, 21, 43, 45, 10, 18, 47, 1,  54, 9,  57, 0,  35,
    62, 31, 40, 4,  49, 5,  52, 26, 60, 6,  23, 44, 46, 27, 56, 16,
    7,  39, 48, 24, 59, 14, 12, 55, 38, 28, 58, 20, 37, 17, 36, 8};

int POP(std::uint64_t& bb) { return PopBit(bb); }
int CNT(std::uint64_t b) { return CountBits(b); }

int PopBit(std::uint64_t& bb) {
  std::uint64_t b = bb ^ (bb - 1);
  std::uint32_t fold = static_cast<std::uint32_t>((b & 0xffffffff) ^ (b >> 32));
  bb &= (bb - 1);
  return BitTable[(fold * 0x783a9b23) >> 26];
}

int CountBits(std::uint64_t b) {
  int r = 0;
  while (b) {
    b &= b - 1;
    ++r;
  }
  return r;
}

void PrintBitBoard(std::uint64_t bb) {
  std::cout << '\n';
  for (int rank = RANK_8; rank >= RANK_1; --rank) {
    for (int file = FILE_A; file <= FILE_H; ++file) {
      int sq = FR2SQ(file, rank);
      int sq64 = SQ64(sq);

      if ((1ULL << sq64) & bb) {
        std::cout << 'X';
      } else {
        std::cout << '-';
      }
    }
    std::cout << '\n';
  }
  std::cout << "\n\n";
}