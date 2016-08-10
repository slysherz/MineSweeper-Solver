#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

//
#define MINE 'M'
#define OUT_OF_BOUNDS 'E'
#define CLEAR 'C'
#define UNKNOWN '?'

// Play results
#define NEUTRAL 0
#define WON 1
#define LOST 2

#define MAYBE_ON 0
#define MAYBE(x) if (MAYBE_ON) x

#define MAX(a, b) b > a ? b : a
#define MIN(a, b) b < a ? b : a


/** POSSIBLE IMPROVEMENTS
 * Decouple the solver from the game engine (makePlay and alikes)
 * Add code to GitHub
 * Split functionality into multiple files
 * clean main()
 * find a way to include the group with all the cells (maybe use a pointer in the struct instaed of 2D array)



/****************************** AUTOMATED SOLVER ******************************/
int solveMineSweeper(Board boardh, Board boardv);


/******************************* USER INTERFACE *******************************/

void getMineSweeperInput (int *x, int *y, char *played);

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

void getMineSweeperInput (int *x, int *y, char *played) {
	char separator[16];
	
	puts("\nInput:");
	while (1) {
		
		int scanned = scanf("%c%16[^0-9]%d%16[^0-9]%d", played, separator, x, separator, y);
		fseek(stdin, 0, SEEK_END);	// Flush new line
		
		if (scanned > 0) {
			// Check that parameters make sense
			printf("\nGot: %c at (%d, %d)", *played, *x, *y);
			break;
		}
		
		puts("Invalid intput, please try again: ");
	}
}


int solveMineSweeper(Board boardh, Board boardv) {
	// Initialize groups
	groupListArrayNumber = 0;
	for (int x = 0; x < boardv.width; x++) {
		for (int y = 0; y < boardv.height; y++) {
			char cell = get(boardv, x,y);
			
			if (cell == MINE || cell == UNKNOWN) {	// Not a number!
				continue;
			}
			
			Group newGroup = buildGroup(boardv, x, y);
			if (newGroup.size != 0) {
				addToList(newGroup);
			}
		}
	}

	while (1) {
		
MAYBE(printf("creation of groups complete\n"));
		for (int i = 0; i < groupListArrayNumber; i++) {
			for (int j = i + 1; j < groupListArrayNumber; j++) {
MAYBE(printf("G%d[%d/%d]; G%d[%d/%d]; GLAN: %d)\n", i, groupList[i].mines, groupList[i].size, j, groupList[j].mines, groupList[j].size, groupListArrayNumber));
				int x;
				if (groupList[i].size == groupList[j].size	// Groups are
				&& containedIn(groupList[i], groupList[j])) {	// the same
					removeFromList(j);
MAYBE(printf("remove\n"));
				} 
				else if (containedIn(groupList[i], groupList[j])) { 
					//remove elements of i from j
MAYBE(printf("SUB: G%d - G%d\n", j, i));
MAYBE(printGroup(groupList[j]));
MAYBE(printGroup(groupList[i]));
					groupList[j] = subtract(groupList[j],groupList[i]);
MAYBE(printGroup(groupList[j]));
				}
				else if (containedIn(groupList[j], groupList[i])) {
					//remove elements of j from i
					
MAYBE(printf("SUB: G%d - G%d\n", i, j));
MAYBE(printGroup(groupList[i]));
MAYBE(printGroup(groupList[j]));
					groupList[i] = subtract(groupList[i],groupList[j]);
MAYBE(printGroup(groupList[i]));
				}
				else if (x = intersect(groupList[i], groupList[j])) {
MAYBE(printf("FIRST: %d, SECOND: %d\n", groupList[i].mines - (groupList[i].size - x), groupList[j].mines - (groupList[j].size - x)));
					int minmine = MAX(
						groupList[i].mines - (groupList[i].size - x), 
						(MAX( 
							groupList[j].mines - (groupList[j].size - x), 
							0
						))
					);
					
					int maxmine = MIN(
						groupList[i].mines, 
						(MIN( 
							groupList[j].mines, 
							x
						))
					);

MAYBE(printf("\nINTERCEPTION: min: %d max: %d\n", minmine, maxmine));
MAYBE(printGroup(groupList[i]));
MAYBE(printGroup(groupList[j]));

					if (minmine == maxmine) {	// IF BUG, CHECK HERE!
						Group a, b;
						a = subtract(groupList[i], groupList[j]);	// LEFT
						b = subtract(groupList[i], a);  //isto vai dar merda!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						b.mines = minmine;
						addToList(b);
						groupList[i] = subtract(groupList[i], b);
						groupList[j] = subtract(groupList[j], b);	// 00:56 29/07/2016: Moreno firstly used spaces here
					} 
					else {
						continue;
					}												// disapointed with the date-time structure standards used by Ribeiro
				}
				else {
					continue;
				}
				
				i = -1;	// Reset outer for
				break;
			}
		}

MAYBE(printf("cleaning of groups complete\n"));
		bool stallAlert = 0;
		int forMax = groupListArrayNumber;
		for (int i = 0; i < forMax; i++){
MAYBE(printf("for i:%d forMax:%d\n",i,forMax));
			int result = NEUTRAL;
			if (groupList[i].mines == 0) {
				for (int  j =0; j < groupList[i].size; j++) {
					makePlay(boardh, boardv, groupList[i].positions[j][0], groupList[i].positions[j][1], CLEAR);
MAYBE(printf("Marking Clear (%d,%d)\n", groupList[i].positions[j][0], groupList[i].positions[j][1]));
				}
			} 
			else if (groupList[i].mines == groupList[i].size) {
				for (int  j = 0; j < groupList[i].size; j++) {
					makePlay(boardh, boardv, groupList[i].positions[j][0], groupList[i].positions[j][1], MINE);
MAYBE(printf("Marking Mine (%d,%d)\n", groupList[i].positions[j][0], groupList[i].positions[j][1]));
					
				}
				removeFromList(i);
				i--;
				forMax--;
			} 
			else {
				continue;
			}
			
			// Delete group
			stallAlert = 1;
			// maybe break;
		}
		
		for (int i = 0; i < forMax; i++){
			int result = NEUTRAL;
			if (groupList[i].mines == 0) {
				for (int  j =0; j < groupList[i].size; j++) {
					Group newGroup = buildGroup(boardv, groupList[i].positions[j][0], groupList[i].positions[j][1]); 
					if (newGroup.size > 0) {
						addToList(newGroup);
					}
				}
				
				// Delete group
				removeFromList(i);
				i--;
				forMax--;
			} 
		}
		
		printBoard(boardv);
		
		if (stallAlert == 0) {
			printBoard(boardv);
			printBoard(boardh);
			printf("Stalled!\n glan: %d \n", groupListArrayNumber);
			
			for (int i = 0; i < groupListArrayNumber; i++) {
				printGroup(groupList[i]);
			}
			return NEUTRAL;
		}
	}
}
