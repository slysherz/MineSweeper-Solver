#pragma once

#include <board_tools.h>

// A group is a list with 'size' cells, which contains 'mines' mines
typedef struct Group_s {
	int mines;
	int *positions;
	int size;
	struct Group_s *next;
} Group;

Group* buildGroup (Board board, int x, int y);

void printGroup (Group *group);

// Is cell (x, y) contained in group?
bool cellContainedIn (int x, int y, Group *group);

// Is group a contained in group b?
bool containedIn (Group *a, Group *b);

// Removes all 'group' cells from 'from'
void subtract (Group *from, Group *group);

// Returns the amount of cells 'a' has in common with 'b'
int intersect(Group *a, Group *b);

//copy group
Group* groupCopy(Group *group);
