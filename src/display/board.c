#include "board.h"
#include "colored.h"
#include <stdint.h>

/**
 * piece_to_char() - get character representation of piece
 * @piece: integer value corresponding to the piece
 *
 * Returns: ' ' if @piece is blank, 'X' if @piece is invalid, else FEN
 * representation of that piece
 */
static char piece_to_char(uint8_t piece)
{
	switch (piece) {
	case PIECE_NONE:
		return ' ';
	case PIECE_BLACK | PIECE_PAWN:
		return 'p';
	case PIECE_BLACK | PIECE_KNIGHT:
		return 'n';
	case PIECE_BLACK | PIECE_BISHOP:
		return 'b';
	case PIECE_BLACK | PIECE_ROOK:
		return 'r';
	case PIECE_BLACK | PIECE_QUEEN:
		return 'q';
	case PIECE_BLACK | PIECE_KING:
		return 'k';
	case PIECE_WHITE | PIECE_PAWN:
		return 'P';
	case PIECE_WHITE | PIECE_KNIGHT:
		return 'N';
	case PIECE_WHITE | PIECE_BISHOP:
		return 'B';
	case PIECE_WHITE | PIECE_ROOK:
		return 'R';
	case PIECE_WHITE | PIECE_QUEEN:
		return 'Q';
	case PIECE_WHITE | PIECE_KING:
		return 'K';
	default:
		return 'X';
	}
}

/**
 * piece_color() - get display color for piece
 * @piece: integer value corresponding to the piece
 *
 * Returns: %COLOR_BLACK if @piece is black, %COLOR_WHITE if @piece is white,
 * %COLOR_RED if piece color is invalid
 */
enum display_color piece_color(uint8_t piece)
{
	return IS_BLACK_PIECE(piece) ? COLOR_BLACK :
	       IS_WHITE_PIECE(piece) ? COLOR_WHITE :
				       COLOR_RED;
}

/**
 * cell_color() - get background color for cell
 * @cell_idx: index of cell, A8 being index 0, A7 index 1, H1 is index 63
 *
 * Returns: %COLOR_BLUE if cell at @cell_idx is dark, %COLOR_CYAN otherwise
 */
enum display_color cell_color(uint8_t cell_idx)
{
	uint8_t row = cell_idx / 8, col = cell_idx % 8;
	bool is_dark_cell = (row + col) % 2 == 1;
	return is_dark_cell ? COLOR_BLUE : COLOR_CYAN;
}

void print_board(struct display_board board)
{
	cprintf(COLOR_WHITE, COLOR_BLACK, "  ╔═════════════════╗\n");
	for (uint8_t cell_idx = 0; cell_idx < 64; cell_idx++) {
		uint8_t p = board.board.grid[cell_idx];
		char pchar = piece_to_char(p);
		enum display_color pcolor = piece_color(p),
				   ccolor = cell_color(cell_idx),
				   ncolor = cell_color(cell_idx + 1);

		if (cell_idx % 8 == 7) {
			cprintf(pcolor, ccolor, "%c", pchar);
			cprintf(ccolor, COLOR_BLACK, "▌");
			cprintf(COLOR_WHITE, COLOR_BLACK, "║\n");
			continue;
		}

		if (cell_idx % 8 == 0) {
			cprintf(COLOR_WHITE, COLOR_BLACK, "%d ║",
				8 - cell_idx / 8);
			cprintf(ccolor, COLOR_BLACK, "▐");
		}
		cprintf(pcolor, ccolor, "%c", pchar);
		cprintf(ccolor, ncolor, "▌");
	}
	// printf("1 ║▐.▐.▐.▐.▐.▐.▐.▐.▐║\n");

	printf("  ╚═════════════════╝\n");
	printf("    A B C D E F G H  \n");
}
