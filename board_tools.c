include "board_tools.h"

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
