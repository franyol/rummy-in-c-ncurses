#include "../include/fsm.h"

void fsm_update(FSM *fsm){
	ssize_t next_state;

	if (fsm->len < 1) return; // No states

	if (fsm->current < 0) {
		fsm->states[0].on_enter(&fsm->states[0], NULL);
		fsm->current = 0;
	} else if (fsm->current >= fsm->len) {
		return; // State doesn't exist, do nothing
	}

	next_state = fsm->states[fsm->current].update(&fsm->states[fsm->current]);

	if (next_state != fsm->current) {
		void *arg = NULL;
		fsm->states[fsm->current].on_exit(&fsm->states[fsm->current], &arg);
		fsm->states[next_state].on_enter(&fsm->states[next_state], arg);
		fsm->current = next_state;
	}
}
