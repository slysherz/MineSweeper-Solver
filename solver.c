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
				free(newGroup);
			}
		}
	}

	while (1) {
MAYBE(printf("creation of groups complete\n"));	
		Group *pi = groupHead, *pj = pi->next;

		while (pi!=NULL){
			while(pj!=NULL){
			




//		for (int i = 0; i < groupListArrayNumber; i++) {
//			for (int j = i + 1; j < groupListArrayNumber; j++) {
//MAYBE(printf("G%d[%d/%d]; G%d[%d/%d]; GLAN: %d)\n", i, groupList[i].mines, groupList[i].size, j, groupList[j].mines, groupList[j].size, groupListArrayNumber));
				if (pi->size == pj->size	// Groups are
				&& containedIn(pi, pj)) {	// the same
					removeFromList(pj);
MAYBE(printf("remove\n"));
				} 
				else if (containedIn(pi, pj)) { 
					//remove elements of i from j
//MAYBE(printf("SUB: G%d - G%d\n", j, i));
//MAYBE(printGroup(groupList[j]));
//MAYBE(printGroup(groupList[i]));
					subtract(pj, pi);
//MAYBE(printGroup(groupList[j]));
				}
				else if (containedIn(pj, pi)) {
					//remove elements of j from i
					
//MAYBE(printf("SUB: G%d - G%d\n", i, j));
//MAYBE(printGroup(groupList[i]));
//MAYBE(printGroup(groupList[j]));
					subtract(pi, pj);
//MAYBE(printGroup(groupList[i]));
				}
				else if (intersectSolver(pi, pj)>=0) {
					int x = intersectSolver(pi, pj);
//MAYBE(printf("FIRST: %d, SECOND: %d\n", groupList[i].mines - (groupList[i].size - x), groupList[j].mines - (groupList[j].size - x)));


MAYBE(printf("\nINTERCEPTION: mine:%d\n", x));
//MAYBE(printGroup(groupList[i]));
//MAYBE(printGroup(groupList[j]));


					Group *a;
					a = groupCopy(pi);
					subtract(pi, pj);	// LEFT
					subtract(a, pi);  //isto vai dar merda!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					a->mines = x;
					addToList(a);
					subtract(pj, a);
					pi->mines = pi->mines + pj->mines - x;
				}
				else {
					pj=pj->next;
					continue;
				}
				
				pi=groupHead;	// Reset outer while
				pj=pi->next;
				break;


			}	
			pi=pi->next;
			pj=pi->next;
		}



//			}
//		}

MAYBE(printf("cleaning of groups complete\n"));
		bool stallAlert = 0;
		Group *ptr = groupHead;
		while(ptr!=NULL){


//		for (int i = 0; i < forMax; i++){
//MAYBE(printf("for i:%d forMax:%d\n",i,forMax));
			int result = NEUTRAL;
			if (ptr->mines == 0) {
				for (int  j =0; j < ptr->size*2; j+=2) {
					makePlay(boardh, boardv, ptr->positions[j], ptr->positions[j+1],CLEAR);
//MAYBE(printf("Marking Clear (%d,%d)\n", groupList[i].positions[j][0], groupList[i].positions[j][1]));
				}
			} 
			else if (ptr->mines == ptr->size) {
				for (int  j = 0; j < ptr->size*2; j+=2) {
					makePlay(boardh, boardv, ptr->positions[j], ptr->positions[j+1], MINE);
//MAYBE(printf("Marking Mine (%d,%d)\n", groupList[i].positions[j][0], groupList[i].positions[j][1]));
					
				}
				removeFromList(ptr);
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
		
	//	for (int i = 0; i < forMax; i++){

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
						free(newGroup);
					}
				}
				
				// Delete group
				removeFromList(ptr);
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
