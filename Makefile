all: ms-solver

ms-solver: main.o board.o board_tools.o group.o group_list.o solver.o ui.o
	gcc main.o board.o board_tools.o group.o group_list.o solver.o ui.o -o ms-solver -I.

main.o: main.c
	gcc -c main.c -I.

board.o: board.c
	gcc -c board.c -I.

board_tools.o: board_tools.c
	gcc -c board_tools.c -I.

group.o: group.c
	gcc -c group.c -I.

group_list.o: group_list.c
	gcc -c group_list.c -I.

solver.o: solver.c
	gcc -c solver.c -I.

ui.o: ui.c
	gcc -c ui.c -I.

clean:
	rm -rf *.o ms-solver











