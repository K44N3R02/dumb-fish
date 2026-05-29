#include "fen.h"

/** from_fen() - construct a board from a FEN string
 * @fen: pointer to first character of FEN string
 *
 * Constructs a board from @fen with grid, who to move, castling rights, en
 * passant target square, and half move count for draw.
 *
 * Assumes: @fen is a valid FEN
 * Returns: a board containing information from @fen
 */
struct board from_fen(const char *fen)
{
	struct board board;
	const char *cursor = fen;

	memset(&board, 0, sizeof(board));

	for (uint8_t row = 0, col = 0; *cursor != ' '; cursor++) {
		if ('1' <= *cursor && *cursor <= '8') {
			col += *cursor - '0';
			continue;
		}
		if (*cursor == '/') {
			row++;
			col = 0;
			continue;
		}
		switch (*cursor) {
		case 'p':
			board.grid[row * 8 + col] = PIECE_BLACK | PIECE_PAWN;
			break;
		case 'n':
			board.grid[row * 8 + col] = PIECE_BLACK | PIECE_KNIGHT;
			break;
		case 'b':
			board.grid[row * 8 + col] = PIECE_BLACK | PIECE_BISHOP;
			break;
		case 'r':
			board.grid[row * 8 + col] = PIECE_BLACK | PIECE_ROOK;
			break;
		case 'q':
			board.grid[row * 8 + col] = PIECE_BLACK | PIECE_QUEEN;
			break;
		case 'k':
			board.grid[row * 8 + col] = PIECE_BLACK | PIECE_KING;
			break;
		case 'P':
			board.grid[row * 8 + col] = PIECE_WHITE | PIECE_PAWN;
			break;
		case 'N':
			board.grid[row * 8 + col] = PIECE_WHITE | PIECE_KNIGHT;
			break;
		case 'B':
			board.grid[row * 8 + col] = PIECE_WHITE | PIECE_BISHOP;
			break;
		case 'R':
			board.grid[row * 8 + col] = PIECE_WHITE | PIECE_ROOK;
			break;
		case 'Q':
			board.grid[row * 8 + col] = PIECE_WHITE | PIECE_QUEEN;
			break;
		case 'K':
			board.grid[row * 8 + col] = PIECE_WHITE | PIECE_KING;
			break;
		default:
			break;
		}
		col++;
	}
	cursor++; // move past space

	board.whites_turn = *cursor == 'w';
	cursor += 2;

	board.castling_rights = 0;
	for (; *cursor != ' '; cursor++) {
		if (*cursor == 'K')
			board.castling_rights |= (1 << 3);
		if (*cursor == 'Q')
			board.castling_rights |= (1 << 2);
		if (*cursor == 'k')
			board.castling_rights |= (1 << 1);
		if (*cursor == 'q')
			board.castling_rights |= (1 << 0);
	}
	cursor++; // move past space

	if (*cursor == '-') {
		board.en_passant_square = 64;
		cursor += 2;
	} else {
		board.en_passant_square =
			8 * (8 - cursor[1] + '1') + (cursor[0] - 'a');
		cursor += 3;
	}

	board.half_turn_count = *cursor - '0';
	if (cursor[1] != ' ') {
		board.half_turn_count *= 10;
		board.half_turn_count += cursor[1] - '0';
	}

	return board;
}
