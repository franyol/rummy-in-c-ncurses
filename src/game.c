#include "../include/game_states.h"
#include "../include/tile.h"
#include "../include/colors.h"
#include "../include/print_utils.h"
#include <time.h>
#include <ncurses.h>

DECLARE_DOUBLE_LINKED_LIST(TileDLLNode);
DEFINE_DOUBLE_LINKED_LIST(TileDLLNode);

typedef enum Hand {
	DECK,
	P1,
	P2,
	P3,
	P4,
	BOARD
} Hand;

TileDLLNodeDLLNode *hands = NULL;
int game_running = 0;

/**
 * Returns new head
 */
TileDLLNode* shuffle(TileDLLNode *head) {
	int len;

	len = Tile_dll_len(head);
	for (int i = 0; i < len*2; i++) {
		Tile_dll_swap_nodes(
				Tile_dll_get_by_index(head, i%len),
				Tile_dll_get_by_index(head, rand()%len)
				); 
		while (head->prev != NULL) {
			head = head->prev;
		}
	}
	return head;
}

/**
 * @return y cursor position after the print
 */
int print_hand(TileDLLNode *head, int y, int x) {
	int max_x, max_y;
	int x_start = x;
	getmaxyx(stdscr, max_y, max_x);
	while (head != NULL) {
		if (y > max_y - 5) break;

		head->data.x = x;
		head->data.y = y;
		print_tile(&(head->data));
		
		x += 6;
		head = head->next;
		if (x > max_x - 6) {
			y += 5;
			x = x_start;
		}
	}

	return y;
}

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
		Tile base_tile = {0, 0, 0, BLACK};

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
		hands->data.next = shuffle(hands->data.next);

		free(deck);
		game_running = 1;
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
	
	clear_win();
	print_hand(hands->data.next, 5, 5);
	
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
