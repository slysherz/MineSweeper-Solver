#pragma once

#include <time.h>

#include <board.h>

// Adds num mines to a board
void setMines (Board board, int num);
// Traverses a board and sets each CLEAR to the number of mines around it
void setCellState(Board board);

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
