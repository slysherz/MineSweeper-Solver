#include "solver.h"

extern int groupListArrayNumber;

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
