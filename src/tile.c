#include "../include/tile.h"

const char tileframe[5][7] = {
" ---- ",
"|    |",
"|    |",
"|    |",
" ---- ",
};

void curses_init_tile(void) {
	init_pair(BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair(RED, COLOR_WHITE, COLOR_RED);
	init_pair(YELLOW, COLOR_WHITE, COLOR_YELLOW);
	init_pair(BLUE, COLOR_WHITE, BLUE);
}

void print_tile(const Tile* tile) {
	attrset(A_NORMAL | COLOR_PAIR(tile->color) | A_BOLD);

	for (int i=0; i < 5; i++) {
		move(tile->y+i, tile->x);
		printw("%s", tileframe[i]);
	}
	move(tile->y+2, tile->x+2);
	printw("%d", tile->num);
}

void erase_tile(const Tile *tile) {
	attrset(A_NORMAL | COLOR_PAIR(BLACK));

	for (int i=0; i < 5; i++) {
		move(tile->y+i, tile->x);
		addstr("      ");
	}
}
