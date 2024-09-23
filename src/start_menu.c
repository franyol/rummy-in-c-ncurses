#include "../include/game_states.h"
#include "../include/print_utils.h"
#include <ncurses.h>

void start_menu_on_enter(FSM_State *self, const void *arg) {
	static char *options[] = {"play", "settings", "quit"};
	static StartMenuData smdata = {options, 3, 0, 1110, GAME};
	if (self->data == NULL) self->data = (void*) &smdata;

	if (arg != NULL) {
		smdata.dificulty = ((StartMenuArg*) arg)->dificulty;
		free((StartMenuArg*) arg);
	}
}

int start_menu_update(FSM_State *self) {
	static int y=0, x=0;
	StartMenuData* this = (StartMenuData*) self->data;

	switch(getch()) {
		case KEY_DOWN:
		case 's':
			this->index = (this->index + 1) % this->len;
			break;
		case KEY_UP:
		case 'w':
			this->index = (this->index > 0) ? this->index - 1 : this->len - 1;
			break;
		case '\n':
			switch(this->index) {
				case 0:
					this->toState = GAME;
					return GAME;
				case 1:
					this->toState = SETTINGS;
					return SETTINGS;
				case 2:
					return -1;
				default:
					break;
			}
			break;
		default:
			break;
	}
	flushinp();
	
	erase_options(y, x, this->len);
	getmaxyx(stdscr, y, x);
	print_options(y, x, this->len, this->index, this->options);

	return START_MENU;
}

void start_menu_on_exit(FSM_State *self, void **arg) {
	StartMenuData *this = (StartMenuData*) self->data;
	SettingsArg* seta; 
	GameArg* gamea;

	switch(this->toState) {
		case GAME:
			gamea = malloc(sizeof(GameArg));
			gamea->dificulty = this->dificulty;
			*arg = (void*) gamea;
			break;
		case SETTINGS:
			seta = malloc(sizeof(SettingsArg));
			seta->dificulty = this->dificulty;
			*arg = (void*) seta;
			break;
		default:
			break;
	}
}

