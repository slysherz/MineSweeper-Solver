#include "group_list.h"

//Group groupList[1000000];
extern Group *groupHead;
void addToList (Group group) {
	group->next = groupHead;
	groupHead = group;
}


void removeFromList (Group *group) {
	
	for (int i = index + 1; i < groupListArrayNumber; i++) {
		groupList[i - 1] = groupList[i];
	}
	
	groupListArrayNumber--;
}
