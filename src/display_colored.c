#include "display_colored.h"

/**
 * set_foreground() - override foreground color of terminal
 * @foreground_color: new foreground color
 *
 * Sends ANSI escape sequence for changing foreground color to
 * @foreground_color.
 *
 * Side Effect: this function changes color of text printed to terminal
 */
static void set_foreground(enum display_color foreground_color)
{
	switch (foreground_color) {
	case COLOR_BLACK:
		printf("\x1b[30m");
		break;
	case COLOR_RED:
		printf("\x1b[31m");
		break;
	case COLOR_GREEN:
		printf("\x1b[32m");
		break;
	case COLOR_YELLOW:
		printf("\x1b[33m");
		break;
	case COLOR_BLUE:
		printf("\x1b[34m");
		break;
	case COLOR_MAGENTA:
		printf("\x1b[35m");
		break;
	case COLOR_CYAN:
		printf("\x1b[36m");
		break;
	case COLOR_WHITE:
		printf("\x1b[37m");
		break;
	case COLOR_BRIGHT_BLACK:
		printf("\x1b[1;30m");
		break;
	case COLOR_BRIGHT_RED:
		printf("\x1b[1;31m");
		break;
	case COLOR_BRIGHT_GREEN:
		printf("\x1b[1;32m");
		break;
	case COLOR_BRIGHT_YELLOW:
		printf("\x1b[1;33m");
		break;
	case COLOR_BRIGHT_BLUE:
		printf("\x1b[1;34m");
		break;
	case COLOR_BRIGHT_MAGENTA:
		printf("\x1b[1;35m");
		break;
	case COLOR_BRIGHT_CYAN:
		printf("\x1b[1;36m");
		break;
	case COLOR_BRIGHT_WHITE:
		printf("\x1b[1;37m");
		break;
	}
}

/**
 * set_background() - override background color of terminal
 * @background_color: new background color
 *
 * Sends ANSI escape sequence for changing background color to
 * @background_color.
 *
 * Side Effect: this function changes color of background printed to terminal
 */
static void set_background(enum display_color background_color)
{
	switch (background_color) {
	case COLOR_BLACK:
		printf("\x1b[40m");
		break;
	case COLOR_RED:
		printf("\x1b[41m");
		break;
	case COLOR_GREEN:
		printf("\x1b[42m");
		break;
	case COLOR_YELLOW:
		printf("\x1b[43m");
		break;
	case COLOR_BLUE:
		printf("\x1b[44m");
		break;
	case COLOR_MAGENTA:
		printf("\x1b[45m");
		break;
	case COLOR_CYAN:
		printf("\x1b[46m");
		break;
	case COLOR_WHITE:
		printf("\x1b[47m");
		break;
	case COLOR_BRIGHT_BLACK:
		printf("\x1b[1;40m");
		break;
	case COLOR_BRIGHT_RED:
		printf("\x1b[1;41m");
		break;
	case COLOR_BRIGHT_GREEN:
		printf("\x1b[1;42m");
		break;
	case COLOR_BRIGHT_YELLOW:
		printf("\x1b[1;43m");
		break;
	case COLOR_BRIGHT_BLUE:
		printf("\x1b[1;44m");
		break;
	case COLOR_BRIGHT_MAGENTA:
		printf("\x1b[1;45m");
		break;
	case COLOR_BRIGHT_CYAN:
		printf("\x1b[1;46m");
		break;
	case COLOR_BRIGHT_WHITE:
		printf("\x1b[1;47m");
		break;
	}
}

/**
 * reset_colors() - reset ANSI color overrides
 *
 * Sends ANSI codes for reseting foreground color and reseting background color.
 *
 * Side Effect: this function resets previous color overrides
 */
static void reset_colors(void)
{
	printf("\x1b[39m\x1b[49m");
}

/**
 * cprintf() - print colored text to stdout
 * @foreground_color: color of the text printed
 * @background_color: color of the surface the text is printed
 * @format: format string for output
 * @...: additional arguments
 *
 * Sets the foreground to @foreground_color and background to @background_color
 * with ANSI escape sequences, prints the given @format by substituting
 * following arguments like printf(), then resets foreground and background
 * colors.
 *
 * Side Effect: this function prints text to stdout and resets any previous
 * color changes
 *
 * Returns: number of characters printed, excluding ANSI escape sequences
 */
int32_t cprintf(enum display_color foreground_color,
		enum display_color background_color, const char *format, ...)
{
	va_list ap;

	set_foreground(foreground_color);
	set_background(background_color);

	va_start(ap, format);
	int32_t result = vprintf(format, ap);
	va_end(ap);

	reset_colors();

	return result;
}
