#include "../include/colors.h"

#include <ncurses.h>

void init_pair_colors(void) {
	init_pair(BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair(RED, COLOR_WHITE, COLOR_RED);
	init_pair(YELLOW, COLOR_WHITE, COLOR_YELLOW);
	init_pair(BLUE, COLOR_WHITE, COLOR_BLUE);
}
