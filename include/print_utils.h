#ifndef _PRINT_UTILS_H_
#define _PRINT_UTILS_H_

void clear_win(void);

void erase_options(int y, int x, int len);

void print_options(int y, int x, int len, int index, char **options);

int handle_animation(const struct timeval *dt);

void start_animation(struct timeval duration, int (*anim)
		(struct timeval count, struct timeval duration));

void printw_delimiters(int horizontal, int y, int x);

#endif
