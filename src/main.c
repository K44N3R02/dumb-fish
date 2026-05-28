#include "display/colored.h"

int main(void)
{
	const char *fen =
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	cprintf(COLOR_BLUE, COLOR_RED, "Starting position:\n%s\n", fen);
}
