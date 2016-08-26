#pragma once

#include <stdio.h>

#include <group.h>

#define GSIZE 1000000


// Adds 'group' to the list
void addToList (Group *group);

// Removes the group with index 'index' from the group
void removeFromList (int index);
