#ifndef DF_ENGINE_MOVE_H
#define DF_ENGINE_MOVE_H

#include "engine_board.h"
#include "utils_list.h"
#include <stddef.h>
#include <stdint.h>

/**
 * struct move - internal move representation
 * @origin_index: where the piece that will move is before the move
 * @target_index: where the piece will land after the move
 * @promote_to: if this was a promotion, what pawn promotes to
 * @prev_castling: previous state's castling rights
 * @prev_en_passant: previous state's en passant square index
 * @prev_half_turn: previous state's 50 move rule counter
 *
 * This representation aims for making and unmaking a move, and has enough
 * information to do so.
 */
struct move {
	uint8_t origin_index;
	uint8_t target_index;
	uint8_t promote_to;
	uint8_t prev_captured_piece;
	uint8_t prev_castling;
	uint8_t prev_en_passant;
	uint8_t prev_half_turn;
};

struct move new_move(uint8_t origin_index, uint8_t target_index,
		     const struct board *board);
struct move new_promotion(uint8_t origin_index, uint8_t target_index,
			  uint8_t promotion, const struct board *board);

void make_move(struct board *board, struct move move);
void unmake_move(struct board *board, struct move move);

DECLARE_LIST(move, struct move);

bool get_legal_moves(const struct board *board, struct move_list *moves);

#endif // DF_ENGINE_MOVE_H
