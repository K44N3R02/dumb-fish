#ifndef DF_ENGINE_BOARD_H
#define DF_ENGINE_BOARD_H

#include <stdbool.h>
#include <stdint.h>

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

#endif // DF_ENGINE_BOARD_H
