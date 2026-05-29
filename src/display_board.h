#ifndef DF_DISPLAY_BOARD_H
#define DF_DISPLAY_BOARD_H

#include "common.h"
#include "engine_board.h"
#include "engine_pieces.h"
#include "display_colored.h"
#include <stdio.h>

/**
 * struct display_board - board representation for pretty printing
 */
struct display_board {
	struct board board;
};

void print_board(struct display_board board);

#endif // DF_DISPLAY_BOARD_H
