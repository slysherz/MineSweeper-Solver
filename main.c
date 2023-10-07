#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>

//#include <board.h>
//#include <board_tools.h>
#include "solver.h"
#include "ui.h"

int groupListArrayNumber = 0;

void checkOrQuit(char *argc[], bool cond) {
	printf("usage: \n\t%s -w [width] -h [height] -m [mines] -f [file] -a\n", argc[0]);
	exit(-1);
}

//args: width height mines ...
int main (int argv, char *argc[]) {
	Board boardh; //boardh == board hidden
	Board boardv; //boardv == board visible

	int mines = 10, width = 8, height = 8;
	bool autoSolve = false;
	bool loadFromFile = false;
	const char* file = NULL;
	for (int i = 1; i < argv; i++) {
		if (strcmp(argc[i], "-f") == 0) {
			loadFromFile = true;
			checkOrQuit(argc, i++ < argv);
			file = argc[i];
		} else if (strcmp(argc[i], "-a") == 0) {
			autoSolve = true;
		} else if (strcmp(argc[i], "-w") == 0) {
			checkOrQuit(argc, i++ < argv);
			width = atoi(argc[i]);
		} else if (strcmp(argc[i], "-h") == 0) {
			checkOrQuit(argc, i++ < argv);
			height = atoi(argc[i]);
		} else if (strcmp(argc[i], "-m") == 0) {
			checkOrQuit(argc, i++ < argv);
			mines = atoi(argc[i]);
		}
	}

	if (mines >= height * width * 8 / 10 /*|| mines < 0*/) {
		printf("Too many mines!!!");
		return -1;
	}
	
	boardh.width	= width;
	boardh.height	= height;
	
	char gridh[size(boardh)];
	memset(gridh, 0, sizeof(gridh));
	boardh.grid = &gridh[0];
	
	setMines(boardh, mines);
	
	if (loadFromFile) {
		boardh = buildBoardFromFile(file);
	}
	boardv.width=boardh.width;
	boardv.height=boardh.height;
	
	char gridv[size(boardv)];
	memset(gridv, UNKNOWN, sizeof(gridv));
	boardv.grid = &gridv[0];	
	setCellState(boardh);
	
	int gameResult = NEUTRAL;
	if (autoSolve) {
		makePlay(boardh, boardv, 0, 0, CLEAR);
		printf("Board Initial State: ");
		printBoard(boardv);
		solveMineSweeper(boardh, boardv);
		exit(0);
	}
	
	while (1) {
		char played;
		int x;
		int y;
		
		printBoard(boardv);
		getMineSweeperInput(&x, &y, &played);
		gameResult = makePlay(boardh, boardv, x, y, played);
		
		if (gameResult != NEUTRAL) {
			break;
		}
	}

	if (gameResult == WON) {
		puts("\n\nCongratulatious! You are not a complete idiot!");
	} 
	else if (gameResult == LOST) {
		puts("\n\nRIP: You lost!");
	}
}
