#include "../include/game_states.h"
#include "../include/colors.h"
#include <ncurses.h>

void erase_options(int y, int x, StartMenuData* this) {
	attrset(A_NORMAL | COLOR_PAIR(BLACK));
	for (int i=0; i<this->len; i++) {
		mvprintw(y/2-1+i, x/2-5, "%*s", 50, "");
	}
}

void print_options(int y, int x, StartMenuData* this) {
	for (int i=0; i<this->len; i++) {
		if (this->index == i) {
			attrset(A_STANDOUT | COLOR_PAIR(BLACK));
		} else {
			attrset(A_NORMAL | COLOR_PAIR(BLACK));
		}
		mvprintw(y/2-1+i, x/2-5, "%s", this->options[i]);
	}
}

void start_menu_on_enter(FSM_State *self, const void *arg) {
	static char *options[] = {"play", "settings", "quit"};
	static StartMenuData smdata = {options, 3, 0};
	self->data = (void*) &smdata;
}

int start_menu_update(FSM_State *self) {
	static int y=0, x=0;
	StartMenuData* this = (StartMenuData*) self->data;

	switch(getch()) {
		case 'q':
			return -1;
			break;
		case KEY_DOWN:
			this->index = (this->index + 1) % this->len;
			break;
		case KEY_UP:
			this->index = (this->index > 0) ? this->index - 1 : this->len - 1;
			break;
		default:
			break;
	}
	flushinp();
	
	erase_options(y, x, this);
	getmaxyx(stdscr, y, x);
	print_options(y, x, this);

	return START_MENU;
}

void start_menu_on_exit(FSM_State *self, void **arg) {
}

