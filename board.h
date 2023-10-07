#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>

#define MAX(a, b) b > a ? b : a
#define MIN(a, b) b < a ? b : a

#define MINE 'M'
#define OUT_OF_BOUNDS 'E'
#define CLEAR 'C'
#define UNKNOWN '?'

#define NEUTRAL 0
#define WON 1
#define LOST 2

#define LIMIT 100000

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

Board buildBoardFromFile(const char file[]);

void printBoard(Board board);
