#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

//#include <board.h>
//#include <board_tools.h>
#include <solver.h>
#include <ui.h>

Group *groupHead = NULL;

//args: width height mines ...
int main (int argv, char *argc[]) {
	Board boardh; //boardh == board hidden
	Board boardv; //boardv == board visible

	int mines = 10, width = 8, height = 8;
	bool autoSolve=0;
	
	bool loadFromFile = 0;
	if (strcmp(argc[1], "f") == 0 && (argv==3 || (argv==4 && strcmp(argc[3], "a") == 0))) {
		loadFromFile = 1;
		if (argv == 4) {
			autoSolve = 1;
		}
		
	}
	else if (argv == 4 || (argv == 5 && 0==strcmp("a",argc[4]))) {
		if (argv == 5) {
			autoSolve = 1;
		}
		
		width	= atoi(argc[1]);
		height	= atoi(argc[2]);
		mines	= atoi(argc[3]);
		if (mines >= height * width * 8 / 10 /*|| mines < 0*/) {
			printf("Too many mines!!!");
			return 0;
		}
		
	} 
	else if (argv == 1 || (argv == 2 && 0 == strcmp("a",argc[1]))) {
		if (argv == 2) {
			autoSolve = 1;
		}
	}
	else {
		printf("usage: %s width hight mines\n", argc[0]);
		return -1;
	}
	
	boardh.width	= width;
	boardh.height	= height;
	
	char gridh[size(boardh)];
	memset(gridh, 0, sizeof(gridh));
	boardh.grid = &gridh[0];
	
	setMines(boardh, mines);
	
	if (loadFromFile) {
		boardh = buildBoardFromFile(argc[2]);
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
