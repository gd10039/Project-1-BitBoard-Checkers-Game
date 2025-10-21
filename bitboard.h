#ifndef BITBOARD_H
#define BITBOARD_H
#include <stdint.h>

typedef uint64_t U64;

typedef struct {
    U64 p1, k1;
    U64 p2, k2;
    int turn;
} GS;

U64 dark_mask(void);
U64 file_a(void);
U64 file_h(void);

int sq_of(char f, int r);
void sq_to(int s, char *buf);
GS start_pos(void);
void print_board(const GS *g);
int move_do(GS *g, int from, int to);
int any_moves(const GS *g);
int save_game(const GS *g, const char *path);
int load_game(GS *g, const char *path);

#endif
