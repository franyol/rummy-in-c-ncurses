#include "../include/game_states.h"
#include "../include/print_utils.h"
#include "../include/colors.h"
#include <ncurses.h>

int power(int base, int ex) {
	int result = base;
	if (ex == 0) return 1;
	for (int i=1; i<ex; i++) result*=base;
	return result;
}

void print_dificulty(int y, int x, int dificulty) {
	attrset(A_NORMAL | COLOR_PAIR(BLACK));
	int cur_dif;
	for (int i = 0; i < 4; i++) {
		cur_dif = dificulty % 10;
		move(y/2-1+i, x/2+15);
		printw("%s", 
				(cur_dif == 0) ? "Player" :
				(cur_dif == 1) ? "Easy" :
				(cur_dif == 2) ? "Normal" :
				"Hard");
		dificulty /= 10;
	}
}

void settings_on_enter(FSM_State *self, const void *arg) {
	static char *options[] = {"player 1", "player 2", "player 3", "player 4", "done"};
	static SettingsData sdata = {options, 5, 0, 0};
	if (self->data == NULL) self->data = (void*) &sdata;

	if (arg != NULL) {
		sdata.dificulty = ((SettingsArg*) arg)->dificulty;
		free((SettingsArg*) arg);
	}
}

int settings_update(FSM_State *self) {
	static int y=0, x=0;
	SettingsData* this = (SettingsData*) self->data;

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
				case 1:
				case 2:
				case 3:
					this->dificulty = 
						(this->dificulty/(power(10,this->index)) % 10 >= 3) ?
						this->dificulty - 3*power(10,this->index)
						: this->dificulty + power(10,this->index);
					break;
				case 4:
					erase_options(y, x, this->len);
					return START_MENU;
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
	print_dificulty(y, x, this->dificulty);

	return SETTINGS;
}

void settings_on_exit(FSM_State *self, void **arg) {
	SettingsData *this = (SettingsData*) self->data;
	StartMenuArg *smarg;

	smarg = (StartMenuArg*) malloc(sizeof(StartMenuArg));
	smarg->dificulty = this->dificulty;
	*arg = (void*) smarg;
}
