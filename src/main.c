#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <sys/time.h>
#include "../include/tile.h"

void mainLoop( int freq ) {
	struct timeval elapsed, f, start, end;
	char ch;

	f.tv_sec = 1/freq;
	f.tv_usec = (1000000/freq) % 1000000;

	if (gettimeofday(&start, NULL) != 0) {
		fprintf(stderr, "gettimeofday error\n");
		exit(EXIT_FAILURE);
	}

	Tile myTile = {0, 5, 13, BLACK};

	for (;;) {
		ch = getch();
		if (ch == 'q') break;

		erase_tile(&myTile);
		myTile.x = (myTile.x + 1) % (getmaxx(stdscr)-6);
		myTile.color = (myTile.color + 1) % 4;
		print_tile(&myTile);
	
		do {
			if (gettimeofday(&end, NULL) != 0) {
				fprintf(stderr, "gettimeofday error\n");
				exit(EXIT_FAILURE);
			}

			elapsed.tv_sec = end.tv_sec - start.tv_sec;
			elapsed.tv_usec = end.tv_usec - start.tv_usec;
		} while( (f.tv_sec > elapsed.tv_sec) || 
				(f.tv_sec == elapsed.tv_sec && f.tv_usec > elapsed.tv_usec) );

		start = end;

		refresh();
	}
}

int main() {
	setlocale(LC_ALL, "");
	initscr(); cbreak(); noecho();
	start_color();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	curses_init_tile();

	mainLoop(3);

	endwin();
	exit(EXIT_SUCCESS);
}
