#ifndef _TILE_H_
#define _TILE_H_

#include "../include/colors.h"

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

#endif
