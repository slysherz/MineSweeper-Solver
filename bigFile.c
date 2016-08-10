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

#define GSIZE 1000000

#define MAYBE_ON 0
#define MAYBE(x) if (MAYBE_ON) x

#define MAX(a, b) b > a ? b : a
#define MIN(a, b) b < a ? b : a

typedef struct {
	int width;
	int height;
	char *grid;
} Board ;

/** POSSIBLE IMPROVEMENTS
 * Decouple the solver from the game engine (makePlay and alikes)
 * Add code to GitHub
 * Split functionality into multiple files
 * clean main()
 * find a way to include the group with all the cells (maybe use a pointer in the struct instaed of 2D array)



/****************************** BOARD OPERATIONS ******************************/


// Adds num mines to a board
void setMines (Board board, int num);


// Traverses a board and sets each CLEAR to the number of mines around it
void setCellState(Board board);

void printBoard(Board board);


// Counts cells on the board that have the same type as 'type'
int countCells (Board board, char type);

// Counts cells around a cell that have the same type as 'type'
int countClose (Board board, int x, int y, char type);

// Returns 1 if won, -1 if lost, 0 if neutral
int makePlay (
	Board hiddenBoard, 
	Board visibleBoard, 
	int x, 
	int y, 
	char played
);

int makePlayAround (
	Board hiddenBoard, 
	Board visibleBoard, 
	int x, 
	int y, 
	char played
);

/****************************** AUTOMATED SOLVER ******************************/
// GROUPS
// A group is a list with 'size' cells, which contains 'mines' mines
typedef struct {
	int mines;
	int positions[8][2];
	int size;
} Group ;

void printGroup (Group group);

// Is cell (x, y) contained in group?
bool cellContainedIn (int x, int y, Group group);

// Is group a contained in group b?
bool containedIn (Group a, Group b);

// Removes all 'group' cells from 'from'
Group subtract (Group from, Group group);

// Returns the amount of cells 'a' has in common with 'b'
int intersect(Group a, Group b);

// GROUP LIST
// Simple way to have a list with all the current groups
Group groupList[GSIZE];

// Current size of the list
int groupListArrayNumber = 0;

// Adds 'group' to the list
void addToList (Group group);

// Removes the group with index 'index' from the group
void removeFromList (int index);

// SOLVER
int solveMineSweeper(Board boardh, Board boardv);


Group buildGroup (Board board, int x, int y);

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

int makePlay(
	Board hiddenBoard, 
	Board visibleBoard, 
	int x, 
	int y, 
	char played
) {
	char hiddenCell = get(hiddenBoard, x, y);
	char visibleCell = get(visibleBoard, x, y);
	
	// Check that we're playing on a valid cell
	if (visibleCell == OUT_OF_BOUNDS) {
		// Out of the board!
		return NEUTRAL;
	} else if (visibleCell != MINE && visibleCell != UNKNOWN) {
		// Already cleared this!
		return NEUTRAL;
	} else if (played != MINE && played != UNKNOWN && played != CLEAR) {
		// Invalid character!
		return NEUTRAL;
	}
	
	if (played == CLEAR) {
		if (hiddenCell == MINE) {
			set(visibleBoard, x, y, 'X');
			printf("\nHidden Board state: ");
			printBoard(hiddenBoard);
			printf("\nVisible Board state: ");
			printBoard(visibleBoard);
			puts("YOU LOST! GO PLAY BARBIE INSTEAD, IT'S MORE YOUR STYLE!");
			exit(0);
		}
		else {
			set(visibleBoard, x, y, get(hiddenBoard, x, y));
		}
		
		// Check if game is over
		if (countCells(hiddenBoard, MINE) 
		==	countCells(visibleBoard, UNKNOWN) + countCells(visibleBoard, MINE)) {
			printBoard(hiddenBoard);
			printBoard(visibleBoard);
			puts("YOU ONLY WIN WHEN I SAY SO... AND I WON'T.");
			exit(0);
		}
		
		return NEUTRAL;
	}
	else {
		set(visibleBoard, x, y, played);
	}
	
	return NEUTRAL;
}

int makePlayAround (
	Board hiddenBoard, 
	Board visibleBoard, 
	int x, 
	int y, 
	char played
) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!(i == 0 && j == 0) && get(visibleBoard, x+i, y+j) == UNKNOWN) {
				int result = makePlay(hiddenBoard, visibleBoard, x+i, y+j, played);
				if (result != NEUTRAL) {
					return result;
				}
			}
		}		 
	}
	
	return NEUTRAL;
}

void printBoard(Board board) {
	puts("\n");
	
	for (int y = 0; y < board.height; y++) {
		for (int x = 0; x < board.width; x++) {
			printf("%c", get(board, x, y));
		}
		printf("\n");
	}
}

bool checkBounds (Board board, int x, int y) {
	return x >= 0
		&& x < board.width
		&& y >= 0 
		&& y < board.height;
}

char get (Board board, int x, int y) {
	if (!checkBounds(board, x, y)) {
		return OUT_OF_BOUNDS;
	}
	
	return board.grid[x + y * board.width];
}

void set (Board board, int x, int y, char value) {
	if (!checkBounds(board, x, y)) {
		return;
	}
	
	board.grid[x + y * board.width] = value;
}

int countCells (Board board, char type) {
	int counter = 0;
	for (int x = 0; x < board.width; x++) {
		for (int y = 0; y < board.height; y++) {
			if (type == get(board, x, y)) {
				counter++;
			}
		}
	}
	
	return counter;
}

int countClose (Board board, int x, int y, char type) {
	int bombs = 0;
	
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!(i == 0 && j == 0) && get(board, x+i, y+j) == type) {
				bombs++;
			}
		}
	}
	
	return bombs;
}

void setCellState(Board board) {
	for (int x = 0; x < board.width; x++) {
		for (int y = 0; y < board.height; y++) {
			if (get(board, x, y) != MINE && get(board, x, y) != OUT_OF_BOUNDS) {
				set(board, x, y, '0' + countClose(board, x, y, MINE));
			}
		}
	}
	return;
}

int size(Board board) {
	return board.width * board.height;
}

void setMines(Board board, int num) {
	srand(time(NULL));

	while (num > 0) {
		int r = rand() % size(board);
		if (get(board, r % board.width, r / board.width) != MINE){
			set(board, r % board.width, r / board.width, MINE);
			num--;
		}
	}
	
	return;
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

Group buildGroup(Board board, int x, int y) {
	Group group;
	
	group.mines = (int) get(board, x, y) - (int) '0' - countClose(board, x, y, MINE);
	int n = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!(i == 0 && j == 0) && get(board, x+i, y+j) == UNKNOWN) {
				group.positions[n][0] = x + i;
				group.positions[n][1] = y + j;
				n++;
			}
		}
	}
	group.size = n;

	return group;
}

//Group groupList[1000000];
//int groupListArrayNumber = 0;
void addToList (Group group) {
	if (groupListArrayNumber>=GSIZE){
		printf("groupList full!!!");
		exit(0);
	}
	groupList[groupListArrayNumber] = group;
	groupListArrayNumber++;
}

void printGroup (Group group) {
	printf("--- %d/%d ---\n", group.mines, group.size);
	
	for (int i = 0; i < group.size; i++) {
		printf("(%d, %d), ", group.positions[i][0], group.positions[i][1]);
	}
	
	printf("\n");
} 

// from = from - group
Group subtract (Group from, Group group) {
	for (int i = 0; i < group.size; i++) {
		for (int j = 0; j < from.size; j++) {
			if (group.positions[i][0] == from.positions[j][0] 
			&& 	group.positions[i][1] == from.positions[j][1]) {
				
				int last = from.size - 1;
				from.positions[j][0] = from.positions[last][0];
				from.positions[j][1] = from.positions[last][1];
				
				from.size--;
				
				break;
			}
		}
	}
	
	from.mines -= group.mines;
	
	return from;
}

void removeFromList (int index) {
	if (index >= groupListArrayNumber) {
		printf("OUT OF BOUNDS! Index: %d", index);
		return;
	}
	
	for (int i = index + 1; i < groupListArrayNumber; i++) {
		groupList[i - 1] = groupList[i];
	}
	
	groupListArrayNumber--;
}

bool cellContainedIn (int x, int y, Group group) {
	for (int i = 0; i < group.size; i++) {
		if (x == group.positions[i][0] && y == group.positions[i][1]) {
			return 1;
		}
	}
	
	return 0;
}

bool containedIn (Group a, Group b) {
	if (a.size > b.size) {
		return 0;
	}
	
	for (int i = 0; i < a.size; i++) {
		if (!cellContainedIn(a.positions[i][0], a.positions[i][1], b)) {
			return 0;
		}
	}
	
	return 1;
}

int intersect(Group a, Group b) {
	int n=0;
	for(int i=0;i<a.size;i++) {
		if (cellContainedIn(a.positions[i][0], a.positions[i][1], b)) {
			n++;
		}
	}
	return n;
}

Board buildBoardFromFile (char *file) {
	printf("Building board from file: %s\n", file);
	
	FILE *filePtr;
	if ((filePtr = fopen(file, "r")) == NULL)
	{
		printf("Problem, cannot open %s for reading.\n", file);
		exit(0);
	}
	else {
		char buff[GSIZE];
		int width = 0;
		int height = 0;
		while (fgets(buff, GSIZE, filePtr) != NULL)
		{
			for (int x = 0; x < strlen(buff); x++) {
				if(buff[x] == '\n' || buff[x] == '\r') {
					buff[x] = '\0';
				}
			}
			
			width = MAX(width, strlen(buff));
			height++;
		}
		
		// Create array
		Board board;
		board.width = width;
		board.height = height;
		char grid[size(board)];	// BUG? Doesn't this go out of scope?
		memset(grid, OUT_OF_BOUNDS, sizeof(grid));
		board.grid = &grid[0];
		
		rewind(filePtr);
		
		printf("Width: %d, Height: %d\n", width, height);
		for (int y = 0; y < height; y++) {
			fgets(buff, GSIZE, filePtr);
			
			for (int x = 0; x < strlen(buff); x++) {
				if(buff[x] == '\n' || buff[x] == '\r') {
					break;
				}
				
				set(board, x, y, buff[x]);
			}
		}
		fclose(filePtr);
		
		return board;
    }
}





/** END **/
