#include "defs.h"

std::array<int, 13> PieceBig = {false, false, true, true, true, true, true,
                                false, true,  true, true, true, true};
std::array<int, 13> PieceMaj = {false, false, false, false, true, true, true,
                                false, false, false, true,  true, true};
std::array<int, 13> PieceMin = {false, false, true, true,  false, false, false,
                                false, true,  true, false, false, false};
std::array<int, 13> PieceVal = {0,   100, 325, 325, 550,  1000, 50000,
                                100, 325, 325, 550, 1000, 50000};
std::array<int, 13> PieceCol = {BOTH,  WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                                BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};

std::array<int, 13> PiecePawn = {false, true,  false, false, false,
                                 false, false, true,  false, false,
                                 false, false, false};
std::array<int, 13> PieceKnight = {false, false, true,  false, false,
                                   false, false, false, true,  false,
                                   false, false, false};
std::array<int, 13> PieceKing = {false, false, false, false, false, false, true,
                                 false, false, false, false, false, true};
std::array<int, 13> PieceRookQueen = {false, false, false, false, true,
                                      true,  false, false, false, false,
                                      true,  true,  false};
std::array<int, 13> PieceBishopQueen = {false, false, false, true,  false,
                                        true,  false, false, false, true,
                                        false, true,  false};
std::array<bool, 13> PieceSlides{false, false, false, true, true, true, false,
                                 false, false, true,  true, true, false};