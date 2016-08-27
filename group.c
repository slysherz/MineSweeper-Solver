#include "group.h"

void freeGroup(Group *group){
	free(group->positions);
	free(group);
}

Group* groupCopy(Group *group){
	Group *copy = (Group*)malloc(sizeof(Group));
	memcpy(copy, group, sizeof(Group));
	int *pos = (int*)malloc(sizeof(int)*2*group->size);
	memcpy(pos, group->positions, sizeof(int)*2*group->size);
	copy->positions=pos;
	return copy;
}

Group* buildGroup(Board board, int x, int y) {
	Group *group;
	group = (Group*)malloc(sizeof(Group));
	//printf("group ptr: %d",*group);
	group->mines = (int) get(board, x, y) - (int) '0' - countClose(board, x, y, MINE);
	group->size = countClose(board, x, y, UNKNOWN);
	int *pos = (int*)malloc(sizeof(int)*2*group->size);
	int s = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!(i == 0 && j == 0) && get(board, x+i, y+j) == UNKNOWN) {
				pos[s] = x + i;
				s++;
				pos[s] = y + j;
				s++;
			}
		}
	}
	group->positions = pos;
	
	return group;
}

void printGroup (Group *group) {
	printf("--- %d/%d ---\n", group->mines, group->size);
	
	for (int i = 0; i < group->size*2; i+=2) {
		printf("(%d, %d), ", group->positions[i], group->positions[i+1]);
	}
	
	printf("\n");
} 

// from = from - group
void subtract (Group *from, Group *group) {
	for (int i = 0; i < group->size*2; i+=2) {
		for (int j = 0; j < from->size*2; j+=2) {
			if (group->positions[i] == from->positions[j] && group->positions[i+1] == from->positions[j+1]) {
				
				int last = 2*(from->size - 1);
				from->positions[j] = from->positions[last];
				from->positions[j+1] = from->positions[last+1];
				
				from->size--;
				
				break;
			}
		}
	}
	
	from->mines -= group->mines;
}

bool cellContainedIn (int x, int y, Group *group) {
	for (int i = 0; i < group->size*2; i+=2) {
		if (x == group->positions[i] && y == group->positions[i+1]) {
			return 1;
		}
	}
	
	return 0;
}

bool containedIn (Group *a, Group *b) {
	if (a->size > b->size) {
		return 0;
	}
	
	for (int i = 0; i < a->size*2; i+=2) {
		if (!cellContainedIn(a->positions[i], a->positions[i+1], b)) {
			return 0;
		}
	}
	
	return 1;
}

int intersect(Group *a, Group *b) {
	int n=0;
	for(int i = 0; i < a->size*2; i+=2) {
		if (cellContainedIn(a->positions[i], a->positions[i+1], b)) {
			n++;
		}
	}
	return n;
}
