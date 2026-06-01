#include "engine_board.h"
#include "engine_move.h"
#include "engine_pieces.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * new_move() - construct a move without promotion
 * @origin_index: where the piece is located on @board.grid
 * @target_index: where the piece will go
 * @board: current game state before move is played
 *
 * Constructs a move which is not a promotion and saves relevant information
 * from @board to undo this move after it is made.
 *
 * Assumes: this function is just a wrapper for long &struct move initialization
 * and does not checks if the move is actually valid.
 *
 * Returns: a new &struct move from @origin_index to @target_index
 */
struct move new_move(uint8_t origin_index, uint8_t target_index,
		     struct board board)
{
	struct move move;
	move.origin_index = origin_index;
	move.target_index = target_index;
	move.promote_to = PIECE_NONE;
	move.prev_captured_piece = board.grid[move.target_index];
	move.prev_castling = board.castling_rights;
	move.prev_en_passant = board.en_passant_square;
	move.prev_half_turn = board.half_turn_count;
	return move;
}

/**
 * new_promotion() - construct a move with promotion
 * @origin_index: where the piece is located on @board.grid
 * @target_index: where the piece will go
 * @promotion: what to promote the piece to after it is moved
 * @board: current game state before move is played
 *
 * Constructs a move which is not a promotion and saves relevant information
 * from @board to undo this move after it is made. Adds promotion information
 * so after piece is moved, it will be changed to @promotion.
 *
 * Assumes: this function is just a wrapper for long &struct move initialization
 * and does not checks if the move is actually valid.
 *
 * Returns: a new &struct move from @origin_index to @target_index
 */
struct move new_promotion(uint8_t origin_index, uint8_t target_index,
			  uint8_t promotion, const struct board board)
{
	struct move move;
	move.origin_index = origin_index;
	move.target_index = target_index;
	move.promote_to = promotion;
	move.prev_captured_piece = board.grid[move.target_index];
	move.prev_castling = board.castling_rights;
	move.prev_en_passant = board.en_passant_square;
	move.prev_half_turn = board.half_turn_count;
	return move;
}

/**
 * make_move() - make a move on the board
 * @board: pointer to board to make @move on
 * @move: move to make on @board
 *
 * Remove the piece at @move.origin_index. If move is not promotion, put that
 * piece at @move.target_index. If move is a promotion, put @move.promote_to at
 * @move.target_index. Finally, change who plays next turn.
 *
 * Assumes: @move should be a valid move on @board
 * Side Effect: this function mutates @board
 */
void make_move(struct board *board, struct move move)
{
	uint8_t moving_piece = board->grid[move.origin_index];

	board->grid[move.target_index] =
		move.promote_to == PIECE_NONE ? moving_piece : move.promote_to;
	board->grid[move.origin_index] = PIECE_NONE;
	board->whites_turn = !board->whites_turn;

	// castling
	if (moving_piece == (PIECE_WHITE | PIECE_KING) &&
	    move.origin_index == SQ_E1 && move.target_index == SQ_G1) {
		board->grid[SQ_H1] = PIECE_NONE;
		board->grid[SQ_F1] = PIECE_WHITE | PIECE_ROOK;
	}
	if (moving_piece == (PIECE_WHITE | PIECE_KING) &&
	    move.origin_index == SQ_E1 && move.target_index == SQ_C1) {
		board->grid[SQ_A1] = PIECE_NONE;
		board->grid[SQ_D1] = PIECE_WHITE | PIECE_ROOK;
	}
	if (moving_piece == (PIECE_BLACK | PIECE_KING) &&
	    move.origin_index == SQ_E8 && move.target_index == SQ_G8) {
		board->grid[SQ_H8] = PIECE_NONE;
		board->grid[SQ_F8] = PIECE_BLACK | PIECE_ROOK;
	}
	if (moving_piece == (PIECE_BLACK | PIECE_KING) &&
	    move.origin_index == SQ_E8 && move.target_index == SQ_C8) {
		board->grid[SQ_A8] = PIECE_NONE;
		board->grid[SQ_D8] = PIECE_BLACK | PIECE_ROOK;
	}
	// we know if something from king's start position moves, it is either
	// king's first move or king has already moved, so castling was either
	// should be disabled or is already disabled
	if (move.origin_index == SQ_E1) {
		DISABLE_WHITE_CASTLE_KING_SIDE(*board);
		DISABLE_WHITE_CASTLE_QUEEN_SIDE(*board);
	}
	if (move.origin_index == SQ_E8) {
		DISABLE_BLACK_CASTLE_KING_SIDE(*board);
		DISABLE_BLACK_CASTLE_QUEEN_SIDE(*board);
	}
	// similar reasoning with rooks
	if (move.origin_index == SQ_A1)
		DISABLE_WHITE_CASTLE_QUEEN_SIDE(*board);
	if (move.origin_index == SQ_H1)
		DISABLE_WHITE_CASTLE_KING_SIDE(*board);
	if (move.origin_index == SQ_A8)
		DISABLE_BLACK_CASTLE_QUEEN_SIDE(*board);
	if (move.origin_index == SQ_H8)
		DISABLE_BLACK_CASTLE_KING_SIDE(*board);
	// if something is going into the rooks' starting positions, either
	// rooks are being captured and castling should disable or rooks are
	// already moved and castling is already disabled
	if (move.target_index == SQ_A1)
		DISABLE_WHITE_CASTLE_QUEEN_SIDE(*board);
	if (move.target_index == SQ_H1)
		DISABLE_WHITE_CASTLE_KING_SIDE(*board);
	if (move.target_index == SQ_A8)
		DISABLE_BLACK_CASTLE_QUEEN_SIDE(*board);
	if (move.target_index == SQ_H8)
		DISABLE_BLACK_CASTLE_KING_SIDE(*board);

	// en passant
	if (moving_piece == (PIECE_WHITE | PIECE_PAWN) &&
	    GRID_ROW(move.origin_index) == 2 &&
	    GRID_ROW(move.target_index) == 4)
		board->en_passant_square =
			GRID_MAKE(GRID_COL(move.origin_index), 3);
	else if (moving_piece == (PIECE_BLACK | PIECE_PAWN) &&
		 GRID_ROW(move.origin_index) == 7 &&
		 GRID_ROW(move.target_index) == 5)
		board->en_passant_square =
			GRID_MAKE(GRID_COL(move.origin_index), 6);
	else
		board->en_passant_square = SQ_NONE;

	// half-turn counter
	board->half_turn_count++;
	if ((moving_piece & PIECE_TYPE_MASK) == PIECE_PAWN) // pawn move
		board->half_turn_count = 0;
	if (move.prev_captured_piece != PIECE_NONE) // capture
		board->half_turn_count = 0;
}

/**
 * unmake_move() - undo a move on the board
 * @board: pointer to the board to undo the @move on
 * @move: move to undo on @board
 *
 * Put piece at @move.target_index back to @move.origin_index and restore the
 * piece at @move.target_index. Then restore @board.castling_rights,
 * @board.en_passant_square, and @board.half_turn_count. Finally, change who
 * plays next turn.
 *
 * Side Effect: this function mutates @board
 */
void unmake_move(struct board *board, struct move move)
{
	uint8_t moving_piece = board->grid[move.target_index];

	board->whites_turn = !board->whites_turn;

	board->grid[move.origin_index] =
		move.promote_to == PIECE_NONE ? board->grid[move.target_index] :
		board->whites_turn	      ? PIECE_WHITE | PIECE_PAWN :
						PIECE_BLACK | PIECE_PAWN;
	board->grid[move.target_index] = move.prev_captured_piece;
	board->castling_rights = move.prev_castling;
	board->en_passant_square = move.prev_en_passant;
	board->half_turn_count = move.prev_half_turn;

	if (moving_piece == (PIECE_WHITE | PIECE_KING) &&
	    move.origin_index == SQ_E1 && move.target_index == SQ_G1) {
		board->grid[SQ_F1] = PIECE_NONE;
		board->grid[SQ_H1] = PIECE_WHITE | PIECE_ROOK;
	}
	if (moving_piece == (PIECE_WHITE | PIECE_KING) &&
	    move.origin_index == SQ_E1 && move.target_index == SQ_C1) {
		board->grid[SQ_D1] = PIECE_NONE;
		board->grid[SQ_A1] = PIECE_WHITE | PIECE_ROOK;
	}
	if (moving_piece == (PIECE_BLACK | PIECE_KING) &&
	    move.origin_index == SQ_E8 && move.target_index == SQ_G8) {
		board->grid[SQ_F8] = PIECE_NONE;
		board->grid[SQ_H8] = PIECE_BLACK | PIECE_ROOK;
	}
	if (moving_piece == (PIECE_BLACK | PIECE_KING) &&
	    move.origin_index == SQ_E8 && move.target_index == SQ_C8) {
		board->grid[SQ_D8] = PIECE_NONE;
		board->grid[SQ_A8] = PIECE_BLACK | PIECE_ROOK;
	}
}
