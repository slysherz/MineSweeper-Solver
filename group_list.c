#include "group_list.h"

//Group groupList[1000000];
extern Group *groupHead;
void addToList (Group *group) {
	group->next = groupHead;
	groupHead = group;
}


Group* removeFromList (Group *group) {

	Group *ptr = groupHead;
	while(ptr->next != group && ptr->next != NULL){
		ptr=ptr->next;	
	}
	ptr->next=group->next;
	freeGroup(group);
	return ptr->next;
}
void printGroupList(){
	Group *ptr=groupHead;
	while(ptr!=NULL){
		printGroup(ptr);
		ptr=ptr->next;
	}
}

