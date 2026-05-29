#include "display/colored.h"
#include "display/board.h"
#include "utils/fen.h"

int main(void)
{
	cprintf(COLOR_GREEN, COLOR_BLACK, "Nimzo-Indian Defense:\n");
	const char *fen =
		"rnbqk2r/pppp1ppp/4pn2/8/1bPP4/2N5/PP2PPPP/R1BQKBNR w KQkq - 2 4";
	struct board board = from_fen(fen);

	struct display_board dboard = (struct display_board){ board };
	print_board(dboard);
}
