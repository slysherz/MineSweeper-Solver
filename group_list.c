#include "group_list.h"

//Group groupList[1000000];
extern Group *groupHead;
void addToList (Group *group) {
	group->next = groupHead;
	groupHead = group;
}


Group* removeFromList (Group *group) {

	Group **ptr = &groupHead;
	while(*ptr != group){
		ptr=&((*ptr)->next);	
	}
	*ptr=(*ptr)->next;
	freeGroup(group);
	return (*ptr)->next;
}
void printGroupList(){
	Group *ptr=groupHead;
	while(ptr!=NULL){
		printGroup(ptr);
		ptr=ptr->next;
	}
}

