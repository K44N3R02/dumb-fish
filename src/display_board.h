#ifndef DF_DISPLAY_BOARD_H
#define DF_DISPLAY_BOARD_H

#include "engine_board.h"

/**
 * struct display_board - board representation for pretty printing
 */
struct display_board {
	struct board board;
};

void print_board(struct display_board board);

#endif // DF_DISPLAY_BOARD_H
