#ifndef DF_DISPLAY_COLORED_H
#define DF_DISPLAY_COLORED_H

#include <stdarg.h>
#include <stdio.h>

#include "common.h"

/**
 * enum display_color - encode ANSI escape colors
 */
enum display_color {
	COLOR_BLACK,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_BLUE,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_WHITE,
	COLOR_BRIGHT_BLACK,
	COLOR_BRIGHT_RED,
	COLOR_BRIGHT_GREEN,
	COLOR_BRIGHT_YELLOW,
	COLOR_BRIGHT_BLUE,
	COLOR_BRIGHT_MAGENTA,
	COLOR_BRIGHT_CYAN,
	COLOR_BRIGHT_WHITE,
};

int32_t cprintf(enum display_color foreground_color,
		enum display_color background_color, const char *format, ...);

#endif // DF_DISPLAY_COLORED_H
