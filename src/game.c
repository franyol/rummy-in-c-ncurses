#include "../include/game_states.h"
#include "../include/tile.h"
#include "../include/colors.h"
#include "../include/print_utils.h"
#include <time.h>
#include <ncurses.h>

extern Hand board_index;
Hand saved_board_index;
int game_running = 0;
int shuflag = 0;

TileDLLNode *selectedTile, *saved_selectedTile;
int grabbed = 0, saved_grabbed;
int on_player_hand = 1, saved_on_player_hand;

NodeStateDLLNode *(saved_states[30]);
int len_saved_states = 0;

enum {
	SHUFFLE,
	PLAYERMOVE,
	COMMOVE,
	TURN_CHANGE
} turn_state = SHUFFLE;

extern TileDLLNodeDLLNode *hands;
extern Hand cur_player;

State shuffle_game(GameData *this);

void save_hands_state(void);
void load_hands_state(void);

void handle_down_press(void);
void handle_up_press(void);

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
		Tile base_tile = {17, 17, 3, 3, 0, BLACK};

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
	int x, y;
	int up_press = 0;
	int down_press = 0;
	int right_press = 0;
	int left_press = 0;
	int select_press = 0;
	int next_turn_press = 0;
	int draw_tile_press = 0;
	int restart_turn_press = 0;
	int save_state_press = 0;

	switch(getch()) {
		case 'q':
			clear_win();
			this->toState = START_MENU;
			game_running = 0;
			return START_MENU;
		case 'p':
			clear_win();
			this->toState = PAUSE_MENU;
			return PAUSE_MENU;
		case KEY_UP:
		case 'w':
			up_press = 1;
			break;
		case KEY_DOWN:
		case 's':
			down_press = 1;
			break;
		case KEY_RIGHT:
		case 'd':
			right_press = 1;
			break;
		case KEY_LEFT:
		case 'a':
			left_press = 1;
			break;
		case '\n':
		case ' ':
			select_press = 1;
			break;
		case 'r':
			restart_turn_press = 1;
			break;
		case 't':
			save_state_press = 1;
			break;
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
		case TURN_CHANGE:
			getmaxyx(stdscr, y, x);
			mvprintw(y/2-3, x/2-10, "It's Player %d's turn!", (cur_player - P1+1)); 
			mvprintw(y/2-1, x/2-11, "Press Enter to continue"); 
			if (select_press) {
				turn_state = 
				(get_dificulty(this->dificulty, cur_player) == 0) ? PLAYERMOVE : COMMOVE;
				selectedTile = GET_HAND(cur_player);
				on_player_hand = 1;
				save_hands_state();
			}
			return GAME;
		case PLAYERMOVE:
			if (selectedTile != NULL && right_press && selectedTile->next != NULL) {
				if (grabbed) {
					if (selectedTile->prev == NULL
							&& selectedTile->next != NULL){
						GET_HAND((on_player_hand) ? cur_player : board_index) =
							selectedTile->next;
					}
					Tile_dll_swap_nodes(selectedTile, selectedTile->next);
				} else
					selectedTile = selectedTile->next; 
			} else if (selectedTile != NULL && left_press && selectedTile->prev != NULL) {
				if (grabbed) {
					if (selectedTile->prev != NULL 
							&& selectedTile->prev->prev == NULL) {
						GET_HAND((on_player_hand) ? cur_player : board_index) = 
							selectedTile;
					}
						
					Tile_dll_swap_nodes(selectedTile, selectedTile->prev);
				} else
					selectedTile = selectedTile->prev; 
			} else if (down_press) {
				handle_down_press();
			} else if (up_press) {
				handle_up_press();
			} else if (select_press) {
				grabbed = (grabbed) ? 0 : 1;
			} else if (restart_turn_press) {
				load_hands_state();
			} else if (save_state_press) {
				save_hands_state();
			}
			place_board();
			if (selectedTile != NULL ) {
				selectedTile->data.y--;
				if (grabbed) {selectedTile->data.x-=2; selectedTile->data.y--;}
			}
			printw_board();
			
			return GAME;
		default:
			place_board();
			break;
	}

	printw_board();
	
	return GAME;
}

void game_on_exit(FSM_State *self, void **arg) {
	GameData* this = (GameData*) self->data;
	TileDLLNodeDLLNode *node;
	StartMenuArg* smarg;
	PauseMenuArg* pmarg;

	if (!game_running) {
		for (node=hands;node!=NULL;node=node->next) {
			Tile_free_all(node->data.next);
		}
		TileDLLNode_free_all(node);
	}

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
	duration.tv_sec = 1;
	duration.tv_usec = 0;
	if (shuflag < 1) {
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 2) {
		place_hand(hands->data.next, 3, 17, false);
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 3) {
		TileDLLNode_dll_sync(hands);
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 4) {
		hands->data.next = shuffle(hands->data.next);
		place_hand(GET_HAND(DECK), 3, 17, false);
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 5) {
		TileDLLNode_dll_sync(hands);
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 6) {
		node = GET_HAND(DECK);
		for(;node!=NULL;node=node->next) {
			node->data.x = 17;
			node->data.y = 3;
		}
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 7) {
		TileDLLNode_dll_sync(hands);
		start_animation(duration, animate_hands);
		shuflag++;
	} else if (shuflag < 8) {
		Tile temp = {0, 0, 0, 0, 0, BLACK};
		TileDLLNode *draw, *head;
		for (Hand i=P1; i <= P4; i++) {
			head = Tile_create_new_node(temp);
			for (int j=0; j < 14; j++) {
				draw = Tile_dll_get_by_index(GET_HAND(DECK),
						rand() % Tile_dll_len(GET_HAND(DECK)));
				if (draw->prev == NULL) GET_HAND(DECK) = draw->next;
				Tile_pop_from_list(draw);
				Tile_dll_append(head, draw);
			}
			head->next->prev = NULL;
			TileDLLNode_dll_append(hands, TileDLLNode_create_new_node(*head));
			free(head);
		}
		// Init board
		draw = Tile_create_new_node(temp);
		TileDLLNode_dll_append(hands, TileDLLNode_create_new_node(*draw));
		free(draw);
		
		place_board();
		start_animation(duration, animate_board);
		turn_state = TURN_CHANGE;
		shuflag++;
	} else {
		turn_state = TURN_CHANGE;
	}
	return GAME;
}

void save_hands_state(void) {
	TileDLLNodeDLLNode *hand;
	int i = 0;
	for (hand=hands; hand!=NULL; hand=hand->next, i++) {
		dll_save_state(TileDLLNode, hand->data.next, saved_states[i]);
		hand->data.prev = hand->data.next;
	}
	len_saved_states = i;
	saved_selectedTile = selectedTile;
	saved_board_index = board_index;
	saved_on_player_hand = on_player_hand;
	saved_grabbed = grabbed;
}

void load_hands_state(void) {
	TileDLLNodeDLLNode *hand, *next;
	int i = 0;
	for (hand=hands; hand!=NULL && i<len_saved_states; hand=hand->next, i++) {
		dll_load_state(TileDLLNode, saved_states[i]);
		hand->data.next = hand->data.prev;
	}
	if (i < TileDLLNode_dll_len(hands)) {
		hand->prev->next = NULL;
		for (; hand != NULL; hand = next) {
			next = hand->next;
			free(hand);
		}
	}
	selectedTile = saved_selectedTile;
	board_index = saved_board_index;
	grabbed = saved_grabbed;
	on_player_hand = saved_on_player_hand;
}

void handle_down_press(void) {
	if (on_player_hand) {
		on_player_hand = 0;
		if (!grabbed)
			selectedTile = Tile_dll_get_by_index(
					GET_HAND(board_index), Tile_dll_idx(selectedTile));
		else if (selectedTile != NULL) {
			if (selectedTile->prev == NULL)
				GET_HAND(cur_player) = selectedTile->next;
			Tile_pop_from_list(selectedTile);
			Tile_dll_append(
					&(TileDLLNode_dll_get_by_index(hands,board_index)->data),
					selectedTile);
			GET_HAND(board_index)->prev = NULL;
		}
	} else if ((int) board_index < TileDLLNode_dll_len(hands)-1) {
		board_index++;
		if (!grabbed)
			selectedTile = Tile_dll_get_by_index(
					GET_HAND(board_index), Tile_dll_idx(selectedTile));
		else if (selectedTile != NULL) {
			if (selectedTile->prev == NULL)
				GET_HAND(board_index-1) = selectedTile->next;
			Tile_pop_from_list(selectedTile);
			Tile_dll_append(
					&(TileDLLNode_dll_get_by_index(hands,board_index)->data),
					selectedTile);
			GET_HAND(board_index)->prev = NULL;
		}
	} else if (grabbed && (int) board_index == TileDLLNode_dll_len(hands)-1
			&& GET_HAND(board_index) != selectedTile) {
		TileDLLNodeDLLNode *temp;
		Tile_pop_from_list(selectedTile);
		GET_HAND(board_index)->prev = NULL;
		board_index++;
		temp = (TileDLLNodeDLLNode*) malloc(sizeof(TileDLLNodeDLLNode));
		temp->data.prev = NULL;
		temp->data.next = selectedTile;
		TileDLLNode_dll_append(hands, temp);
	}
}

void handle_up_press(void) {
	if (!on_player_hand && board_index > BOARD) {
		board_index--;
		if (!grabbed)
			selectedTile = Tile_dll_get_by_index(
					GET_HAND(board_index), Tile_dll_idx(selectedTile));
		else if (selectedTile != NULL) {
			if (selectedTile->prev == NULL)
				GET_HAND(board_index+1) = selectedTile->next;
			Tile_pop_from_list(selectedTile);
			Tile_dll_append(
					&(TileDLLNode_dll_get_by_index(hands,board_index)->data),
					selectedTile);
			GET_HAND(board_index)->prev = NULL;
		}
	} else {
		on_player_hand = 1;
		if (!grabbed)
			selectedTile = Tile_dll_get_by_index(
					GET_HAND(cur_player), Tile_dll_idx(selectedTile));
		else if (selectedTile != NULL) {
			if (selectedTile->prev == NULL)
				GET_HAND(board_index) = selectedTile->next;
			Tile_pop_from_list(selectedTile);
			Tile_dll_append(
					&(TileDLLNode_dll_get_by_index(hands,cur_player)->data),
					selectedTile);
			GET_HAND(cur_player)->prev = NULL;
			selectedTile = Tile_dll_get_by_index(
					GET_HAND(cur_player), Tile_dll_idx(selectedTile));
		}
	}
}
