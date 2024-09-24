#ifndef _TILE_H_
#define _TILE_H_

#include "../include/colors.h"
#include "../include/double_linked_lists.h"

typedef struct Tile {
	int x;
	int prevx;
	int y;
	int prevy;
	int num;
	Color color;
} Tile;

DECLARE_DOUBLE_LINKED_LIST(Tile);

/**
 *	Prints tile to screen
 */
void print_tile(const Tile* tile);

/**
 *	Erases tile from screen
 */
void erase_tile(const Tile* tile);

#endif
