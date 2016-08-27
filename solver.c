#include "solver.h"

extern Group *groupHead;

int solveMineSweeper(Board boardh, Board boardv) {
	// Initialize groups
	for (int x = 0; x < boardv.width; x++) {
		for (int y = 0; y < boardv.height; y++) {
			char cell = get(boardv, x,y);
			
			if (cell == MINE || cell == UNKNOWN) {	// Not a number!
				continue;
			}
			
			Group *newGroup = buildGroup(boardv, x, y);
			if (newGroup->size != 0) {
				addToList(newGroup);
			}
			else {
				freeGroup(newGroup);
			}
		}
	}

	while (1) {
MAYBE(printf("creation of groups complete\n"));	



		cleanEtAll(groupHead);

MAYBE(printf("cleaning of groups complete\n"));
		bool stallAlert = 0;
		Group *ptr = groupHead;
		while(ptr!=NULL){


			int result = NEUTRAL;
			if (ptr->mines == 0) {
				for (int  j =0; j < ptr->size*2; j+=2) {
					makePlay(boardh, boardv, ptr->positions[j], ptr->positions[j+1],CLEAR);
MAYBE(printf("Marking Clear (%d,%d)\n", ptr->positions[j], ptr->positions[j+1]));
										
					Group *newGroup = buildGroup(boardv, ptr->positions[j], ptr->positions[j+1]); 
					if (newGroup->size > 0) {
						addToList(newGroup);
						cleanEtAll(newGroup);
					}
					else {
						freeGroup(newGroup);
					}




				}
				ptr=removeFromList(ptr);
				stallAlert=1;
				continue;
			} 
			else if (ptr->mines == ptr->size) {
				for (int  j = 0; j < ptr->size*2; j+=2) {
					makePlay(boardh, boardv, ptr->positions[j], ptr->positions[j+1], MINE);
MAYBE(printf("Marking Mine (%d,%d)\n", ptr->positions[j], ptr->positions[j+1]));
					
				}
				ptr = removeFromList(ptr);

				stallAlert=1;
				continue;
//MAYBE(printGroupList());
			} 
			ptr=ptr->next;
			
			
		}
		
		printBoard(boardv);
		
		if (stallAlert == 0) {
			printBoard(boardv);
			printBoard(boardh);
			printf("Stalled!\n");
			
			printGroupList();
			return NEUTRAL;
		}
	}
}
int intersectSolver(Group *a, Group *b){
	if (intersect(a, b)){
		int i = intersect(a, b);
		int minmine = MAX(
			a->mines - (a->size - i),
			(MAX( 
				b->mines - (b->size - i), 
				0
			))
		);
		
		int maxmine = MIN(
			a->mines, 
			(MIN( 
				b->mines, 
				i
			))
		);
		if (minmine==maxmine){
			return minmine;
		}
	}
	return -1;
}


bool clean(Group *pi, Group *pj){
	
	if (pi->size == pj->size && containedIn(pi, pj)) {
		removeFromList(pj);
	} 
	else if (containedIn(pi, pj)) { 
					//remove elements of i from j
		subtract(pj, pi);
		cleanEtAll(pj);
	}
	else if (containedIn(pj, pi)) {
					//remove elements of j from i
					
		subtract(pi, pj);
		cleanEtAll(pi);
		return 1;
	}
	else if (intersectSolver(pi, pj)>=0) {
		int x = intersectSolver(pi, pj);

		Group *a;
		a = groupCopy(pi);
		subtract(pi, pj);	// LEFT
		pi->mines = pi->mines + pj->mines - x;
		cleanEtAll(pi);

		subtract(a, pi);  //isto vai dar merda!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		addToList(a);
		cleanEtAll(a);

		subtract(pj, a);
		cleanEtAll(pj);

		return 1;
	}
	return 0;
}

void cleanEtAll(Group *group){
	Group *ptr=groupHead;
	while(ptr!=NULL){
		if(ptr==group){
			ptr=ptr->next;
		}
		if(clean(group, ptr)){
			break;
		}
		ptr=ptr->next;
	}
}

