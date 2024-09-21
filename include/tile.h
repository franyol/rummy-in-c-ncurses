#ifndef _TILE_H_
#define _TILE_H_

#include <ncurses.h>

typedef enum Color {
	BLACK,
	RED,
	BLUE,
	YELLOW
} Color;

typedef struct Tile {
	int x;
	int y;
	int num;
	Color color;
} Tile;

/**
 *	Prints tile to screen
 */
void print_tile(const Tile* tile);

/**
 *	Erases tile from screen
 */
void erase_tile(const Tile* tile);

void curses_init_tile(void);

#endif
