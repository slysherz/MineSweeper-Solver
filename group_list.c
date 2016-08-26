#include "group_list.h"

//Group groupList[1000000];
extern Group *groupHead;
void addToList (Group *group) {
	group->next = groupHead;
	groupHead = group;
}


void removeFromList (Group *group) {

	Group *ptr = groupHead;
	while(ptr->next != group && ptr->next != NULL){
		ptr=ptr->next;	
	}
	ptr->next=group->next;
	free(group);
}
