#pragma once

typedef struct {
	int width;
	int height;
	char *grid;
} Board ;

// Checks if position (x, y) is inside the grid
bool checkBounds (Board board, int x, int y);

// Returns the value of a specific grid cell
char get (Board board, int x, int y);

// Sets the value of a specific grid cell to a new 'value'
void set (Board board, int x, int y, char value);

// Returns the area of the grid
int size(Board board);

Board buildBoardFromFile(char file[]);

void printBoard(Board board);
