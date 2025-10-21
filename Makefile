CC=gcc
CFLAGS=-std=c11 -O2 -Wall -Wextra
OBJS=main.o bitboard.o ui.o

all: checkers

checkers: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c ui.h bitboard.h
bitboard.o: bitboard.c bitboard.h
ui.o: ui.c ui.h bitboard.h

clean:
	rm -f $(OBJS) checkers
