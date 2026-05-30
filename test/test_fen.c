#include "unity.h"
#include "../src/engine_pieces.h"
#include "../src/utils_fen.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void test_from_fen_starting_position(void)
{
	const char *start_fen =
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	struct board b = from_fen(start_fen);

	// 1. Test Turn
	TEST_ASSERT_TRUE_MESSAGE(b.whites_turn, "It should be white's turn");

	// 2. Test Castling Rights (KQkq = 1<<3 | 1<<2 | 1<<1 | 1<<0 = 15)
	uint8_t expected_castling = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);
	TEST_ASSERT_EQUAL_UINT8_MESSAGE(
		expected_castling, b.castling_rights,
		"Castling rights should be full (KQkq)");

	// 3. Test En Passant (None = 64)
	TEST_ASSERT_EQUAL_UINT8_MESSAGE(
		64, b.en_passant_square,
		"En passant square should be 64 (none)");

	TEST_ASSERT_EQUAL_INT_MESSAGE(0, b.half_turn_count,
				      "Half turn count should be 0");

	// Row 0, Col 0 (a8) -> Black Rook
	TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_ROOK, b.grid[0]);

	// Row 0, Col 4 (e8) -> Black King
	TEST_ASSERT_EQUAL_UINT8(PIECE_BLACK | PIECE_KING, b.grid[4]);

	// Row 7, Col 0 (a1) -> White Rook (Row 7 * 8 + 0 = 56)
	TEST_ASSERT_EQUAL_UINT8(PIECE_WHITE | PIECE_ROOK, b.grid[56]);

	// Row 4, Col 4 (e4) -> Empty (0)
	TEST_ASSERT_EQUAL_UINT8(PIECE_NONE, b.grid[4 * 8 + 4]);
}

void test_from_fen_position2(void)
{
	const char *position = "8/8/3p4/1Pp4r/KR3p1k/8/4P1P1/8 w - c6 0 2";
	struct board b = from_fen(position);

	TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, b.castling_rights,
					"There should be no castling right");

	TEST_ASSERT_EQUAL_UINT8_MESSAGE(
		18, b.en_passant_square,
		"C6 (index 18) should be en passant square");
}

void test_from_fen_position3(void)
{
	const char *position = "8/7k/8/3B1K2/5N2/8/8/8 b - - 23 12";
	struct board b = from_fen(position);

	TEST_ASSERT_FALSE_MESSAGE(b.whites_turn, "It should be black's turn");
	TEST_ASSERT_EQUAL_UINT8_MESSAGE(
		23, b.half_turn_count,
		"Position should have 23 plies for 50 move draw");
}

void test_from_fen_position4(void)
{
	const char *position =
		"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
	struct board b = from_fen(position);

	TEST_ASSERT_EQUAL_UINT8_MESSAGE(0b00000011, b.castling_rights,
					"Only black castles should be allowed");
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_from_fen_starting_position);
	RUN_TEST(test_from_fen_position2);
	RUN_TEST(test_from_fen_position3);
	RUN_TEST(test_from_fen_position4);

	return UNITY_END();
}
