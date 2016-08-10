
#define GSIZE 1000000

// Simple way to have a list with all the current groups
Group groupList[GSIZE];

// Current size of the list
int groupListArrayNumber = 0;

// Adds 'group' to the list
void addToList (Group group);

// Removes the group with index 'index' from the group
void removeFromList (int index);
