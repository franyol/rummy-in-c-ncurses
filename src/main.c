#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <sys/time.h>
#include "../include/game_states.h"
#include "../include/colors.h"

void mainLoop( int freq ) {
	struct timeval elapsed, f, start, end;
	FSM_State states[2] = {
	{START_MENU, NULL, start_menu_on_enter, start_menu_on_exit, start_menu_update},
	{SETTINGS, NULL, settings_on_enter, settings_on_exit, settings_update}
	};
	FSM fsm = {states, sizeof(states)/sizeof(FSM_State), -1};

	f.tv_sec = 1/freq;
	f.tv_usec = (1000000/freq) % 1000000;

	if (gettimeofday(&start, NULL) != 0) {
		fprintf(stderr, "gettimeofday error\n");
		exit(EXIT_FAILURE);
	}

	for (;;) {
		fsm_update(&fsm);
		if (fsm.current == -1) break;
	
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
	init_pair_colors();

	mainLoop(15);

	endwin();
	exit(EXIT_SUCCESS);
}
