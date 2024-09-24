#include "../include/tile.h"

#include <ncurses.h>

DEFINE_DOUBLE_LINKED_LIST(Tile);

const char tileframe[5][7] = {
" ---- ",
"|    |",
"|    |",
"|    |",
" ---- ",
};

void print_empty_tile(int y, int x) {
	attrset(A_NORMAL | COLOR_PAIR(BLACK) | A_BOLD);
	for (int i=0; i < 5; i++) {
		move(y+i, x);
		printw("%s", tileframe[i]);
	}
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
