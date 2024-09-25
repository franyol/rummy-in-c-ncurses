#include "../include/game_states.h"
#include "../include/tile.h"
#include "../include/print_utils.h"
#include <time.h>
#include <ncurses.h>

DEFINE_DOUBLE_LINKED_LIST(TileDLLNode);

int ydel = 3;

TileDLLNodeDLLNode *hands = NULL;
Hand cur_player = P1;
extern enum {
	SHUFFLE,
	PLAYERMOVE,
	COMMOVE
} turn_state;

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
int place_hand(TileDLLNode *head, int y, int x, int is_prev) {
	TileDLLNode* node = head;
	int max_x;
	int x_start = x;
	max_x = getmaxx(stdscr);
	while (node != NULL) {
		if (is_prev) {
			node->data.prevx = x;
			node->data.prevy = y;
		} else {
			node->data.x = x;
			node->data.y = y;
		}
		
		x += 6;
		node = node->next;
		if (x > max_x - 10) {
			y += 5;
			x = x_start;
		}
	}

	return y+5;
}

void printw_hand_hidden(TileDLLNode *head) {
	int max_y;
	TileDLLNode* node;
	max_y = getmaxy(stdscr);
	for (node = head; node != NULL; node = node->next) {
		if (node->data.y > max_y - 5) continue;
		print_empty_tile(node->data.y, node->data.x);
	}
}

void place_board(TileDLLNodeDLLNode *board_index) {
	TileDLLNodeDLLNode *hand;
	int cur_y;
	cur_y = place_hand(GET_HAND(cur_player), 3, 17 ,false);
	ydel = cur_y;
	cur_y += 3;

	hand = board_index;
	for(;hand!=NULL;hand=hand->next) {
		cur_y = place_hand(hand->data.next, cur_y, 17, false);
	}
}

void printw_board() {
	TileDLLNodeDLLNode *hand;

	printw_delimiters(0, 3, 16);
	printw_delimiters(1, ydel, 0);

	if (turn_state == PLAYERMOVE) {
		printw_hand(GET_HAND(cur_player));
	} else {
		printw_hand_hidden(GET_HAND(cur_player));
	}

	hand = TileDLLNode_dll_get_by_index(hands, BOARD);
	for(;hand!=NULL;hand=hand->next) {
		printw_hand(hand->data.next);
	}
}

void printw_hand(TileDLLNode *head) {
	int max_y;
	TileDLLNode* node;
	max_y = getmaxy(stdscr);
	for (node = head; node != NULL; node = node->next) {
		if (node->data.y > max_y - 5) continue;
		print_tile(&(node->data));
	}
}

int animate_board(struct timeval count, struct timeval duration) {
	int maxx, maxy;
	float percent = (float) 100 * (count.tv_sec*1000000 + count.tv_usec) / 
		(duration.tv_sec*1000000 + duration.tv_usec);

	TileDLLNode *node;
	TileDLLNodeDLLNode *hand;
	Tile mockTile = {0, 0, 0, 0, 0, BLACK};
	clear_win();
	getmaxyx(stdscr, maxy, maxx);

	printw_delimiters(0, 3, 16);
	printw_delimiters(1, ydel, 0);

	for (node = GET_HAND(cur_player); node != NULL; node = node->next) {
		if (turn_state == PLAYERMOVE) {
			mockTile.num = node->data.num;
			mockTile.color = node->data.color;
		}
		mockTile.x = node->data.prevx + (node->data.x - node->data.prevx)*percent/100;
		mockTile.y = node->data.prevy + (node->data.y - node->data.prevy)*percent/100;
		if (mockTile.x > maxx - 5 || mockTile.y > maxy - 6) continue;
		if (turn_state == PLAYERMOVE) {
			print_tile(&mockTile);
		} else {
			print_empty_tile(mockTile.y, mockTile.x);	
		}
	}

	for (hand = TileDLLNode_dll_get_by_index(hands, BOARD); hand != NULL; hand = hand->next) {
		for (node = hand->data.next; node != NULL; node = node->next) {
			mockTile.num = node->data.num;
			mockTile.color = node->data.color;
			mockTile.x = node->data.prevx + (node->data.x - node->data.prevx)*percent/100;
			mockTile.y = node->data.prevy + (node->data.y - node->data.prevy)*percent/100;
			print_tile(&mockTile);
		}
	}
	return 1;
}

int animate_hands(struct timeval count, struct timeval duration) {
	int maxx, maxy;
	float percent = (float) 100 * (count.tv_sec*1000000 + count.tv_usec) / 
		(duration.tv_sec*1000000 + duration.tv_usec);

	TileDLLNode *node;
	Tile mockTile = {0, 0, 0, 0, 0, BLACK};
	clear_win();
	getmaxyx(stdscr, maxy, maxx);
	for (node = hands->data.next; node != NULL; node = node->next) {
		mockTile.num = node->data.num;
		mockTile.color = node->data.color;
		mockTile.x = node->data.prevx + (node->data.x - node->data.prevx)*percent/100;
		mockTile.y = node->data.prevy + (node->data.y - node->data.prevy)*percent/100;
		if (mockTile.x > maxx - 5 || mockTile.y > maxy - 6) continue;
		print_tile(&mockTile);
	}
	return 1;
}

void Tile_dll_sync(TileDLLNode *head) {
	TileDLLNode* node;
	for (node=head;node!=NULL;node=node->next) {
		node->data.prevy = node->data.y;
		node->data.prevx = node->data.x;
	}
}

void TileDLLNode_dll_sync(TileDLLNodeDLLNode *head) {
	TileDLLNodeDLLNode* node;
	for (node=head;node!=NULL;node=node->next) {
		Tile_dll_sync(node->data.next);
	}
}

int get_dificulty(int dificulty, Hand player) {
	return (int) dificulty/power(10,player - P1) % 10;
}
