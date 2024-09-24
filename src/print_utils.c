#include <ncurses.h>
#include <stdlib.h>
#include "../include/print_utils.h"
#include "../include/colors.h"

void clear_win(void) {
	attrset(A_NORMAL | COLOR_PAIR(BLACK));
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	for (int i=0; i<max_y; i++) {
		mvprintw(i,0,"%*s", max_x, "");
	}
}

void erase_options(int y, int x, int len) {
	attrset(A_NORMAL | COLOR_PAIR(BLACK));
	for (int i=0; i<len; i++) {
		mvprintw(y/2-1+i, x/2-5, "%*s", 50, "");
	}
}

void print_options(int y, int x, int len, int index, char **options) {
	for (int i=0; i<len; i++) {
		if (index == i) {
			attrset(A_STANDOUT | COLOR_PAIR(BLACK));
		} else {
			attrset(A_NORMAL | COLOR_PAIR(BLACK));
		}
		mvprintw(y/2-1+i, x/2-5, "%s", options[i]);
	}
}

int anim_running = 0;
int (*anim_current) (struct timeval dt, struct timeval duration) = NULL;
struct timeval anim_duration;
struct timeval anim_count;

void start_animation(struct timeval duration, int (*anim)(struct timeval, struct timeval)) {
	anim_duration = duration;	
	anim_current = anim;
	anim_count.tv_sec = 0;
	anim_count.tv_usec = 0;
	anim_running = 1;
}

int handle_animation(const struct timeval *dt) {
	anim_count.tv_sec += dt->tv_sec;
	anim_count.tv_usec += dt->tv_usec;
	if (anim_count.tv_sec * 1000000 + anim_count.tv_usec >
			anim_duration.tv_sec * 1000000 + anim_duration.tv_usec) {

		anim_current = NULL;
		anim_duration.tv_usec = 0;
		anim_duration.tv_sec = 0;
		anim_count.tv_sec = 0;
		anim_count.tv_usec = 0;
		return 0;
	}
	if (!anim_running || anim_current == NULL) return 0;
	anim_running = anim_current(anim_count, anim_duration);
	if (!anim_running) {
		anim_current = NULL;
		anim_duration.tv_usec = 0;
		anim_duration.tv_sec = 0;
		anim_count.tv_sec = 0;
		anim_count.tv_usec = 0;
		return 0;
	}
	return anim_running;
}

