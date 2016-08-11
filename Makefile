CC=gcc
CFLAGS=-O3 -std=c99

all: ms-solver

ms-solver: main.o board.o board_tools.o group.o group_list.o solver.o ui.o
	$(CC) main.o board.o board_tools.o group.o group_list.o solver.o ui.o -o ms-solver -I. $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -I. $(CFLAGS)

board.o: board.c
	$(CC) -c board.c -I. $(CFLAGS)

board_tools.o: board_tools.c
	$(CC) -c board_tools.c -I. $(CFLAGS)

group.o: group.c
	$(CC) -c group.c -I. $(CFLAGS)

group_list.o: group_list.c
	$(CC) -c group_list.c -I. $(CFLAGS)

solver.o: solver.c
	$(CC) -c solver.c -I. $(CFLAGS)

ui.o: ui.c
	$(CC) -c ui.c -I. $(CFLAGS)

clean:
	rm -rf *.o ms-solver











