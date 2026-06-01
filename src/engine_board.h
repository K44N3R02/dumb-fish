#ifndef DF_ENGINE_BOARD_H
#define DF_ENGINE_BOARD_H

#include <stdbool.h>
#include <stdint.h>

#define SQ_A8 0
#define SQ_B8 1
#define SQ_C8 2
#define SQ_D8 3
#define SQ_E8 4
#define SQ_F8 5
#define SQ_G8 6
#define SQ_H8 7
#define SQ_A7 8
#define SQ_B7 9
#define SQ_C7 10
#define SQ_D7 11
#define SQ_E7 12
#define SQ_F7 13
#define SQ_G7 14
#define SQ_H7 15
#define SQ_A6 16
#define SQ_B6 17
#define SQ_C6 18
#define SQ_D6 19
#define SQ_E6 20
#define SQ_F6 21
#define SQ_G6 22
#define SQ_H6 23
#define SQ_A5 24
#define SQ_B5 25
#define SQ_C5 26
#define SQ_D5 27
#define SQ_E5 28
#define SQ_F5 29
#define SQ_G5 30
#define SQ_H5 31
#define SQ_A4 32
#define SQ_B4 33
#define SQ_C4 34
#define SQ_D4 35
#define SQ_E4 36
#define SQ_F4 37
#define SQ_G4 38
#define SQ_H4 39
#define SQ_A3 40
#define SQ_B3 41
#define SQ_C3 42
#define SQ_D3 43
#define SQ_E3 44
#define SQ_F3 45
#define SQ_G3 46
#define SQ_H3 47
#define SQ_A2 48
#define SQ_B2 49
#define SQ_C2 50
#define SQ_D2 51
#define SQ_E2 52
#define SQ_F2 53
#define SQ_G2 54
#define SQ_H2 55
#define SQ_A1 56
#define SQ_B1 57
#define SQ_C1 58
#define SQ_D1 59
#define SQ_E1 60
#define SQ_F1 61
#define SQ_G1 62
#define SQ_H1 63
#define SQ_NONE 64

#define GRID_MAKE(col, row) (col + (8 - row) * 8)

#define GRID_ROW(square) (8 - (square) / 8)
#define GRID_COL(square) ((square) % 8)
#define GRID_COL_LETTER(square) (((square) % 8) + 'a')

/**
 * struct board - internal representation of a chess board for searches
 * @grid: array of chess pieces on the board
 * @whites_turn: whether the next turn is white's
 * @castling_rights: information about who can castle where
 * @en_passant_square: index of en passant square on @grid
 * @half_turn_count: "ply"s since last pawn move or capture
 */
struct board {
	/**
	 * @grid: stores grid of the board, by concatenating rows from left to
	 * right, from row 8 to row 1, like in FEN representation
	 */
	uint8_t grid[64];
	bool whites_turn;
	/**
	 * @castling_rights: uses this byte's bits to represent castling rights
	 *                   8: white can castle king side
	 *                   4: white can castle queen side
	 *                   2: black can castle king side
	 *                   1: black can castle queen side
	 */
	uint8_t castling_rights;
	/**
	 * @en_passant_square: index of a possible en passant square on @grid,
	 * or 64 if there is no en passant possible
	 */
	uint8_t en_passant_square;
	uint8_t half_turn_count;
};

#define CAN_WHITE_CASTLE_KING_SIDE(board) \
	(((board).castling_rights & 8) ? true : false)
#define CAN_WHITE_CASTLE_QUEEN_SIDE(board) \
	(((board).castling_rights & 4) ? true : false)
#define CAN_BLACK_CASTLE_KING_SIDE(board) \
	(((board).castling_rights & 2) ? true : false)
#define CAN_BLACK_CASTLE_QUEEN_SIDE(board) \
	(((board).castling_rights & 1) ? true : false)

#define DISABLE_WHITE_CASTLE_KING_SIDE(board) \
	((board).castling_rights &= 0xff ^ (1 << 3))
#define DISABLE_WHITE_CASTLE_QUEEN_SIDE(board) \
	((board).castling_rights &= 0xff ^ (1 << 2))
#define DISABLE_BLACK_CASTLE_KING_SIDE(board) \
	((board).castling_rights &= 0xff ^ (1 << 1))
#define DISABLE_BLACK_CASTLE_QUEEN_SIDE(board) \
	((board).castling_rights &= 0xff ^ 1)

#endif // DF_ENGINE_BOARD_H
