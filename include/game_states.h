#ifndef _GAME_STATES_H_
#define _GAME_STATES_H_

#include "fsm.h"

#define DECLARE_STATE_CALLBACKS(name) \
	void name##_on_enter(FSM_State* self, const void *arg); \
	void name##_on_exit(FSM_State* self, void **arg); \
	int name##_update(FSM_State* self); \

DECLARE_STATE_CALLBACKS(start_menu);
DECLARE_STATE_CALLBACKS(game);
DECLARE_STATE_CALLBACKS(pause_menu);

typedef struct StartMenuData {
	char **options;
	int len;
	int index;
} StartMenuData;

typedef struct GameArg {
	int players;
	int dificulty;
} GameArg;

typedef struct PauseMenuArg {
	int dificulty;
} PauseMenuArg;

typedef enum State {
	START_MENU,
	GAME,
	PAUSE_MENU
} State;

#endif
