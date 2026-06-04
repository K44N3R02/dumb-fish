#include "engine_board.h"
#include "engine_move.h"
#include "engine_pieces.h"
#include "utils_list.h"
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
		     const struct board *board)
{
	struct move move;
	move.origin_index = origin_index;
	move.target_index = target_index;
	move.promote_to = PIECE_NONE;
	move.prev_captured_piece = board->grid[move.target_index];
	move.prev_castling = board->castling_rights;
	move.prev_en_passant = board->en_passant_square;
	move.prev_half_turn = board->half_turn_count;
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
			  uint8_t promotion, const struct board *board)
{
	struct move move;
	move.origin_index = origin_index;
	move.target_index = target_index;
	move.promote_to = promotion;
	move.prev_captured_piece = board->grid[move.target_index];
	move.prev_castling = board->castling_rights;
	move.prev_en_passant = board->en_passant_square;
	move.prev_half_turn = board->half_turn_count;
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

DEFINE_LIST(move, struct move);

/**
 * knight_moves() - add pseudo-legal knight moves from a square
 * @board: pointer to &struct board whose legal moves will be generated
 * @color: color of player to move
 * @square: origin square of knight moves
 * @moves: list where pseudo-legal moves will be added
 *
 * NOTE: this function does not consider pins/checks etc.
 *
 * Side Effect: this function adds new &struct move's to @moves
 */
static void knight_moves(const struct board *board, uint8_t color,
			 uint8_t square, struct move_list *moves)
{
	uint8_t row = GRID_ROW(square);
	char col = GRID_COL(square);

	if (row <= (8 - 2) && col <= ('h' - 1)) {
		uint8_t target = square + 2 * OFFSET_UP + OFFSET_RIGHT;
		if (PIECE_COLOR(board->grid[target]) != color)
			push_move_list(moves, new_move(square, target, board));
	}
	if (row <= (8 - 1) && col <= ('h' - 2)) {
		uint8_t target = square + OFFSET_UP + 2 * OFFSET_RIGHT;
		if (PIECE_COLOR(board->grid[target]) != color)
			push_move_list(moves, new_move(square, target, board));
	}
	if (row <= (8 - 2) && col >= ('a' + 1)) {
		uint8_t target = square + 2 * OFFSET_UP + OFFSET_LEFT;
		if (PIECE_COLOR(board->grid[target]) != color)
			push_move_list(moves, new_move(square, target, board));
	}
	if (row <= (8 - 1) && col >= ('a' + 2)) {
		uint8_t target = square + OFFSET_UP + 2 * OFFSET_LEFT;
		if (PIECE_COLOR(board->grid[target]) != color)
			push_move_list(moves, new_move(square, target, board));
	}
	if (row >= (1 + 2) && col <= ('h' - 1)) {
		uint8_t target = square + 2 * OFFSET_DOWN + OFFSET_RIGHT;
		if (PIECE_COLOR(board->grid[target]) != color)
			push_move_list(moves, new_move(square, target, board));
	}
	if (row >= (1 + 1) && col <= ('h' - 2)) {
		uint8_t target = square + OFFSET_DOWN + 2 * OFFSET_RIGHT;
		if (PIECE_COLOR(board->grid[target]) != color)
			push_move_list(moves, new_move(square, target, board));
	}
	if (row >= (1 + 2) && col >= ('a' + 1)) {
		uint8_t target = square + 2 * OFFSET_DOWN + OFFSET_LEFT;
		if (PIECE_COLOR(board->grid[target]) != color)
			push_move_list(moves, new_move(square, target, board));
	}
	if (row >= (1 + 1) && col >= ('a' + 2)) {
		uint8_t target = square + OFFSET_DOWN + 2 * OFFSET_LEFT;
		if (PIECE_COLOR(board->grid[target]) != color)
			push_move_list(moves, new_move(square, target, board));
	}
}

/**
 * sliding_moves() - add pseudo-legal sliding moves from a square in a direction
 * @board: pointer to &struct board whose legal moves will be generated
 * @color: color of player to move
 * @square: origin square of sliding moves
 * @drow: change in row of piece on every step
 * @dcol: change in column of piece on every step
 * @moves: list where pseudo-legal moves will be added
 *
 * Adds @drow and @dcol offsets to square and until destination is not in
 * borders of chess board, and adds this move to @moves list. It early exits
 * before adding a move if it would land on a piece with same color. It early
 * exits after adding a capture move.
 *
 * NOTE: this function does not consider pins/checks etc.
 *
 * Side Effect: this function adds new &struct move's to @moves
 */
static void sliding_moves(const struct board *board, uint8_t color,
			  uint8_t square, int8_t drow, int8_t dcol,
			  struct move_list *moves)
{
	int8_t row = GRID_ROW(square);
	char col = GRID_COL(square);

	row += drow;
	col += dcol;

	while (1 <= row && row <= 8 && 'a' <= col && col <= 'h') {
		uint8_t target = GRID_MAKE(col, row);
		if (PIECE_COLOR(board->grid[target]) == color)
			break; // can't capture friendly piece
		struct move move = new_move(square, target, board);
		push_move_list(moves, move);
		if (move.prev_captured_piece != PIECE_NONE)
			break; // captured enemy piece
		row += drow;
		col += dcol;
	}
}

static void rook_moves(const struct board *board, uint8_t color, uint8_t square,
		       struct move_list *moves)
{
	sliding_moves(board, color, square, 0, +1, moves);
	sliding_moves(board, color, square, 0, -1, moves);
	sliding_moves(board, color, square, +1, 0, moves);
	sliding_moves(board, color, square, -1, 0, moves);
}

static void bishop_moves(const struct board *board, uint8_t color,
			 uint8_t square, struct move_list *moves)
{
	sliding_moves(board, color, square, +1, +1, moves);
	sliding_moves(board, color, square, +1, -1, moves);
	sliding_moves(board, color, square, -1, +1, moves);
	sliding_moves(board, color, square, -1, -1, moves);
}

static void queen_moves(const struct board *board, uint8_t color,
			uint8_t square, struct move_list *moves)
{
	rook_moves(board, color, square, moves);
	bishop_moves(board, color, square, moves);
}

/**
 * get_legal_moves() - get all possible moves on given board
 * @board: pointer to the &struct board whose legal moves will be generated
 * @moves: pointer to a list of moves to return the legal moves
 *
 * Side Effect: this function overrides information in @moves
 * Returns: false if @board.grid is corrupted, true if no problems occurred
 */
bool get_legal_moves(const struct board *board, struct move_list *moves)
{
	free_move_list(moves);
	init_move_list(moves);

	uint8_t color_to_move = board->whites_turn ? PIECE_WHITE : PIECE_BLACK;

	for (uint8_t index = 0; index < 64; index++) {
		uint8_t piece = board->grid[index];
		if (PIECE_COLOR(piece) != color_to_move)
			continue;

		switch (PIECE_TYPE(piece)) {
		case PIECE_PAWN:
			break;
		case PIECE_KNIGHT:
			knight_moves(board, color_to_move, index, moves);
			break;
		case PIECE_BISHOP:
			bishop_moves(board, color_to_move, index, moves);
			break;
		case PIECE_ROOK:
			rook_moves(board, color_to_move, index, moves);
			break;
		case PIECE_QUEEN:
			queen_moves(board, color_to_move, index, moves);
			break;
		case PIECE_KING:
			break;
		default:
			return false;
		}
	}

	return true;
}
