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
		Group *pi = groupHead, *pj; 

		while (pi!=NULL){
//MAYBE(printf("1st while\n"));
			pj=pi->next;
			while(pj!=NULL){
//MAYBE(printf("2nd while\n"));
				if(sanitaize(pi, pj)){
					pj=NULL;
					pi=groupHead;
				}
				else{
					pj=pj->next;
				}
//MAYBE(printGroupList());			
			}	
			pi=pi->next;
		}




MAYBE(printf("cleaning of groups complete\n"));
		bool stallAlert = 0;
		Group *ptr = groupHead;
		while(ptr!=NULL){


			int result = NEUTRAL;
			if (ptr->mines == 0) {
				for (int  j =0; j < ptr->size*2; j+=2) {
					makePlay(boardh, boardv, ptr->positions[j], ptr->positions[j+1],CLEAR);
MAYBE(printf("Marking Clear (%d,%d)\n", ptr->positions[j], ptr->positions[j+1]));
				}
			} 
			else if (ptr->mines == ptr->size) {
				for (int  j = 0; j < ptr->size*2; j+=2) {
					makePlay(boardh, boardv, ptr->positions[j], ptr->positions[j+1], MINE);
MAYBE(printf("Marking Mine (%d,%d)\n", ptr->positions[j], ptr->positions[j+1]));
					
				}
//MAYBE(printGroupList());
MAYBE(printf("before del: %p\n",ptr));
MAYBE(printGroup(ptr));
				ptr = removeFromList(ptr);
MAYBE(printf("after del: %p\n",ptr));
//MAYBE(printGroup(ptr));

				stallAlert=1;
				continue;
//MAYBE(printGroupList());
			} 
			else {
				ptr=ptr->next;
				continue;
			}
			
			// Delete group
			stallAlert = 1;
			// maybe break;
			ptr=ptr->next;
		}
		

		ptr=groupHead;
		while(ptr!=NULL){

			int result = NEUTRAL;
			if (ptr->mines == 0) {
				for (int  j =0; j < ptr->size*2; j+=2) {
					Group *newGroup = buildGroup(boardv, ptr->positions[j], ptr->positions[j+1]); 
					if (newGroup->size > 0) {
						addToList(newGroup);
					}
					else {
						freeGroup(newGroup);
					}
				}
				// Delete group
//MAYBE(printGroupList());
//MAYBE(printGroup(ptr));
MAYBE(printf("before del: %p\n",ptr));
MAYBE(printGroup(ptr));
				ptr=removeFromList(ptr);
MAYBE(printf("after del: %p\n",ptr));
//MAYBE(printGroup(ptr));
				continue;
//MAYBE(printGroupList());
//MAYBE(printGroup(ptr));
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


bool sanitaize(Group *pi, Group *pj){
	
	if (pi->size == pj->size && containedIn(pi, pj)) {
		removeFromList(pj);
	} 
	else if (containedIn(pi, pj)) { 
					//remove elements of i from j
		subtract(pj, pi);
	}
	else if (containedIn(pj, pi)) {
					//remove elements of j from i
					
		subtract(pi, pj);
	}
	else if (intersectSolver(pi, pj)>=0) {
		int x = intersectSolver(pi, pj);
MAYBE(printf("before\n"));
MAYBE(printGroup(pi));
MAYBE(printGroup(pj));

		Group *a;
		a = groupCopy(pi);
MAYBE(printGroup(a));
		subtract(pi, pj);	// LEFT
		subtract(a, pi);  //isto vai dar merda!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		a->mines = x;
		addToList(a);
		subtract(pj, a);
		pi->mines = pi->mines + pj->mines;
MAYBE(printf("after\n"));
MAYBE(printGroup(pi));
MAYBE(printGroup(pj));
MAYBE(printGroup(a));
	}
	else {
		return 0;
	}
	return 1;
}

