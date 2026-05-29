#ifndef DF_ENGINE_PIECES_H
#define DF_ENGINE_PIECES_H

#include "../common.h"

#define PIECE_NONE ((uint8_t)0b00000000)
#define PIECE_PAWN ((uint8_t)0b00000001)
#define PIECE_KNIGHT ((uint8_t)0b00000010)
#define PIECE_BISHOP ((uint8_t)0b00000011)
#define PIECE_ROOK ((uint8_t)0b00000100)
#define PIECE_QUEEN ((uint8_t)0b00000101)
#define PIECE_KING ((uint8_t)0b00000110)
#define PIECE_WHITE ((uint8_t)0b00001000)
#define PIECE_BLACK ((uint8_t)0b00010000)

#define PIECE_TYPE_MASK ((uint8_t)0b00000111)
#define PIECE_COLOR_MASK ((uint8_t)0b00011000)

#define IS_WHITE_PIECE(piece) (((piece) & PIECE_COLOR_MASK) == PIECE_WHITE)
#define IS_BLACK_PIECE(piece) (((piece) & PIECE_COLOR_MASK) == PIECE_BLACK)

#endif // DF_ENGINE_PIECES_H
