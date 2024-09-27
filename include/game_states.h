#ifndef _GAME_STATES_H_
#define _GAME_STATES_H_

#include "fsm.h"
#include "tile.h"

typedef enum State {
	START_MENU,
	SETTINGS,
	GAME,
	PAUSE_MENU
} State;

#define DECLARE_STATE_CALLBACKS(name) \
	void name##_on_enter(FSM_State* self, const void *arg); \
	void name##_on_exit(FSM_State* self, void **arg); \
	int name##_update(FSM_State* self, struct timeval *dt); \

DECLARE_STATE_CALLBACKS(start_menu);
DECLARE_STATE_CALLBACKS(game);
DECLARE_STATE_CALLBACKS(settings);
DECLARE_STATE_CALLBACKS(pause_menu);

typedef struct StartMenuData {
	char **options;
	int len;
	int index;
	int dificulty;
	State toState;
} StartMenuData;

typedef struct PauseMenuData {
	char **options;
	int len;
	int index;
	int dificulty;
	State toState;
} PauseMenuData;

typedef struct SettingsData {
	char **options;
	int len;
	int index;
	int dificulty;
	State fromState;
} SettingsData;

typedef struct GameData {
	int dificulty;
	State toState;
} GameData;

typedef struct StartMenuArg {
	int dificulty;
} StartMenuArg;

typedef struct SettingsArg {
	int dificulty;
	State fromState;
} SettingsArg;

typedef struct GameArg {
	int dificulty;
} GameArg;

typedef struct PauseMenuArg {
	int dificulty;
} PauseMenuArg;

/** Game State declarations **/

#define GET_HAND(hand) TileDLLNode_dll_get_by_index(hands, hand)->data.next

DECLARE_DOUBLE_LINKED_LIST(TileDLLNode);
TileDLLNode* shuffle(TileDLLNode *head);

typedef enum Hand {
	DECK,
	P1,
	P2,
	P3,
	P4,
	BOARD
} Hand;

int place_hand(TileDLLNode *head, int y, int x, int is_prev);

void printw_hand(TileDLLNode *head);
void printw_hand_hidden(TileDLLNode *head);
void printw_board();
void place_board();

int hand_ok(TileDLLNode *head, const char ** message);
int is_trio(TileDLLNode *head);
int is_straight(TileDLLNode *head);

int animate_hands(struct timeval count, struct timeval duration);
int animate_board(struct timeval count, struct timeval duration);

void Tile_dll_sync(TileDLLNode *head);

void TileDLLNode_dll_sync(TileDLLNodeDLLNode *head);

int power(int base, int ex);
int get_dificulty(int dificulty, Hand player);

#endif
