#ifndef UI_H
#define UI_H
#include "bitboard.h"
int parse_move(const char *s, int *from, int *to);
void loop_game(void);
#endif
