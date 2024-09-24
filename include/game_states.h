#ifndef _GAME_STATES_H_
#define _GAME_STATES_H_

#include "fsm.h"

typedef enum State {
	START_MENU,
	SETTINGS,
	GAME,
	PAUSE_MENU
} State;

#define DECLARE_STATE_CALLBACKS(name) \
	void name##_on_enter(FSM_State* self, const void *arg); \
	void name##_on_exit(FSM_State* self, void **arg); \
	int name##_update(FSM_State* self); \

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

#endif
