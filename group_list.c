#include "group_list.h"

//Group groupList[1000000];
extern int groupListArrayNumber;
void addToList (Group group) {
	if (groupListArrayNumber>=GSIZE){
		printf("groupList full!!!");
		exit(0);
	}
	groupList[groupListArrayNumber] = group;
	groupListArrayNumber++;
}


void removeFromList (int index) {
	if (index >= groupListArrayNumber) {
		printf("OUT OF BOUNDS! Index: %d", index);
		return;
	}
	
	for (int i = index + 1; i < groupListArrayNumber; i++) {
		groupList[i - 1] = groupList[i];
	}
	
	groupListArrayNumber--;
}
