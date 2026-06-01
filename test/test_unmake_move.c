#include "engine_board.h"
#include "engine_move.h"
#include "engine_pieces.h"
#include "utils_fen.h"

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

/*
 * What should I test for unmake_move()?
 * - does captured piece reappear
 * - do castling rights get restored
 * - does half-turn counter get restored
 */

void test_make_move_position1(void)
{
	const char *
		start_fen =
		       "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
	       *next_fen =
		       "rnbqkbnr/pp1ppppp/2p5/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_C7, SQ_C6, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undoing to double pawn move must re-enable en passant");
}

void test_make_move_position2(void)
{
	const char *
		start_fen =
		       "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
	       *next_fen =
		       "rnbqkb1r/pppppppp/5n2/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 1 1";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_G8, SQ_F6, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undoing to non-pawn move must re-disable en passant");
}

void test_make_move_position3(void)
{
	const char *start_fen = "7r/1KP5/4k3/8/8/8/8/3R4 w - - 0 1",
		   *next_fen = "2Q4r/1K6/4k3/8/8/8/8/3R4 b - - 0 1";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_promotion(
		SQ_C7, SQ_C8, PIECE_WHITE | PIECE_QUEEN, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(&board_before, &board_after,
					 sizeof(struct board),
					 "undoing promotion must restore pawn");
}

void test_make_move_position4(void)
{
	const char *
		start_fen =
		       "rnbqk2r/ppp1ppbp/3p1np1/8/3PP3/2N2N2/PPP1BPPP/R1BQK2R b KQkq - 3 5",
	       *next_fen =
		       "rnbq1rk1/ppp1ppbp/3p1np1/8/3PP3/2N2N2/PPP1BPPP/R1BQK2R w KQ - 4 6";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_E8, SQ_G8, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undoing short castling must move rook too");
}

void test_make_move_position5(void)
{
	const char *
		start_fen =
		       "rn1q1rk1/pbp1ppbp/1p1p1np1/8/3PP3/2N1BN2/PPPQBPPP/R3K2R w KQ - 2 8",
	       *next_fen =
		       "rn1q1rk1/pbp1ppbp/1p1p1np1/8/3PP3/2N1BN2/PPPQBPPP/2KR3R b - - 3 8";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_E1, SQ_C1, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undoing long castling should move the rook");
}

void test_make_move_position6(void)
{
	const char *
		start_fen =
		       "rnbqkbnr/ppp2ppp/8/3pp3/P6P/8/1PPPPPP1/RNBQKBNR w KQkq - 0 3",
	       *next_fen =
		       "rnbqkbnr/ppp2ppp/8/3pp3/P6P/7R/1PPPPPP1/RNBQKBN1 b Qkq - 1 3";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_H1, SQ_H3, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undoing rook move should give back one castling right");
}

void test_make_move_position7(void)
{
	const char *
		start_fen =
		       "rnbqkb1r/ppppnp1p/6p1/4Q3/4P3/8/PPPP1PPP/RNB1KBNR w KQkq - 1 4",
	       *next_fen =
		       "rnbqkb1Q/ppppnp1p/6p1/8/4P3/8/PPPP1PPP/RNB1KBNR b KQq - 0 4";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_E5, SQ_H8, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undoing a rook capture should restore a castling right");
}

void test_make_move_position8(void)
{
	const char *
		start_fen =
		       "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2",
	       *next_fen =
		       "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPPKPPP/RNBQ1BNR b kq - 1 2";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_E1, SQ_E2, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undoing king move should give back castling rights");
}

// at this point, i am tired of renaming messages

void test_make_move_position9(void)
{
	const char *
		start_fen =
		       "rnbq1bnr/ppppkppp/8/4p3/4P3/8/PPPPKPPP/RNBQ1BNR w - - 2 3",
	       *next_fen =
		       "rnbq1bnr/ppppkppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR b - - 3 3";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_E2, SQ_E1, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undo: castling should be disallowed even after king returns");
}

void test_make_move_position10(void)
{
	const char *
		start_fen =
		       "rnbqkb1r/ppp2ppp/5n2/3pp3/P6P/R7/1PPPPPP1/1NBQKBNR w Kkq - 2 4",
	       *next_fen =
		       "rnbqkb1r/ppp2ppp/5n2/3pp3/P6P/8/1PPPPPP1/RNBQKBNR b Kkq - 3 4";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_A3, SQ_A1, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undo: castling should be disallowed even after rook returns back");
}

void test_make_move_position11(void)
{
	const char *start_fen = "8/6k1/2R5/1r5P/5PK1/8/8/8 b - - 9 5",
		   *next_fen = "8/5k2/2R5/1r5P/5PK1/8/8/8 w - - 10 6";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_G7, SQ_F7, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undo: non-capture and non-pawn move must increment half-turn counter");
}

void test_make_move_position12(void)
{
	const char *start_fen = "8/5k2/2R5/1r5P/5PK1/8/8/8 w - - 10 6",
		   *next_fen = "8/5k2/2R5/1r3P1P/6K1/8/8/8 b - - 0 6";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_F4, SQ_F5, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undo: pawn move must reset half-turn counter");
}

void test_make_move_position13(void)
{
	const char *start_fen = "8/5k2/1R6/r4P1P/6K1/8/8/8 b - - 6 9",
		   *next_fen = "8/5k2/1R6/5r1P/6K1/8/8/8 w - - 0 10";
	struct board board_before = from_fen(start_fen),
		     board_after = from_fen(next_fen);
	struct move move = new_move(SQ_A5, SQ_F5, board_before);

	unmake_move(&board_after, move);
	TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
		&board_before, &board_after, sizeof(struct board),
		"undo: capture must reset half-turn counter");
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_make_move_position1);
	RUN_TEST(test_make_move_position2);
	RUN_TEST(test_make_move_position3);
	RUN_TEST(test_make_move_position4);
	RUN_TEST(test_make_move_position5);
	RUN_TEST(test_make_move_position6);
	RUN_TEST(test_make_move_position7);
	RUN_TEST(test_make_move_position8);
	RUN_TEST(test_make_move_position9);
	RUN_TEST(test_make_move_position10);
	RUN_TEST(test_make_move_position11);
	RUN_TEST(test_make_move_position12);
	RUN_TEST(test_make_move_position13);

	return UNITY_END();
}
