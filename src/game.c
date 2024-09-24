#include "../include/game_states.h"
#include "../include/tile.h"
#include "../include/colors.h"
#include "../include/print_utils.h"
#include <time.h>
#include <ncurses.h>

int game_running = 0;
int shuflag = 0;

enum {
	SHUFFLE,
	PLAYERMOVE,
	COMMOVE
} turn_state = SHUFFLE;

extern TileDLLNodeDLLNode *hands;

State shuffle_game(GameData *this);

void game_on_enter(FSM_State *self, const void *arg) {
	static GameData gdata = {0, SETTINGS};
	if (self->data == NULL) self->data = (void*) &gdata;

	if (arg != NULL) {
		gdata.dificulty = ((GameArg*) arg)->dificulty;
		free((GameArg*) arg);
	}

	if (!game_running) {
		srand (time(NULL));
		TileDLLNode* deck = NULL;
		Tile base_tile = {5, 5, 3, 3, 0, BLACK};

		// Head is both jockers
		deck = Tile_create_new_node(base_tile);
		Tile_dll_append(deck, Tile_create_new_node(base_tile));
		Tile_dll_append(deck, Tile_create_new_node(base_tile));
		for (Color color = 0; color < 4; color++) {
			for (int num = 1; num <= 13; num++) {
				base_tile.num = num;
				base_tile.color = color;
				Tile_dll_append(deck, Tile_create_new_node(base_tile));
				Tile_dll_append(deck, Tile_create_new_node(base_tile));
			}
		}
		hands = TileDLLNode_create_new_node(*deck);
		hands->data.next->prev = NULL;

		free(deck);
		game_running = 1;
		shuflag = 0;
		turn_state = SHUFFLE;
	}
}

int game_update(FSM_State *self, struct timeval *dt) {
	GameData* this = (GameData*) self->data;

	switch(getch()) {
		case 'q':
			clear_win();
			this->toState = START_MENU;
			return START_MENU;
		case 'p':
			clear_win();
			this->toState = PAUSE_MENU;
			return PAUSE_MENU;
		default:
			break;
	}
	flushinp();
	
	if (handle_animation(dt)) {
		return GAME;
	}

	clear_win();
	switch(turn_state) {
		case SHUFFLE:
			return shuffle_game(this);
		default: break;
	}
	
	return GAME;
}

void game_on_exit(FSM_State *self, void **arg) {
	GameData* this = (GameData*) self->data;
	StartMenuArg* smarg;
	PauseMenuArg* pmarg;

	switch(this->toState) {
		case START_MENU:
			smarg = (StartMenuArg*) malloc(sizeof(StartMenuArg));
			smarg->dificulty = this->dificulty;
			*arg = smarg;
			break;
		case PAUSE_MENU:
			pmarg = (PauseMenuArg*) malloc(sizeof(PauseMenuArg));
			pmarg->dificulty = this->dificulty;
			*arg = pmarg;
			break;
		default:
			break;
	}
}

State shuffle_game(GameData *this) {
	struct timeval duration;
	TileDLLNode *node;
	if (shuflag < 1) {
		duration.tv_sec = 1;
		duration.tv_usec = 0;
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 2) {
		duration.tv_sec = 1;
		duration.tv_usec = 0;
		place_hand(hands->data.next, 3, 5, false);
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 3) {
		duration.tv_sec = 1;
		duration.tv_usec = 0;
		TileDLLNode_dll_sync(hands);
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 4) {
		duration.tv_sec = 1;
		duration.tv_usec = 0;
		hands->data.next = shuffle(hands->data.next);
		place_hand(hands->data.next, 3, 5, false);
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 5) {
		duration.tv_sec = 1;
		duration.tv_usec = 0;
		TileDLLNode_dll_sync(hands);
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 6) {
		duration.tv_sec = 1;
		duration.tv_usec = 0;
		node = hands->data.next;
		for(;node!=NULL;node=node->next) {
			node->data.x = 5;
			node->data.y = 3;
		}
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 7) {
		duration.tv_sec = 1;
		duration.tv_usec = 0;
		TileDLLNode_dll_sync(hands);
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 7) {
		node = hands->data.next;
		//start_animation(duration, animate_hands);
		shuflag++;
	} else {
		turn_state = (this->dificulty % 10 == 0) ? PLAYERMOVE : COMMOVE; 
	}
	return GAME;
}
