#include <ncurses.h>
#include "../include/print_utils.h"
#include "../include/colors.h"

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
