void printGroup (Group group) {
	printf("--- %d/%d ---\n", group.mines, group.size);
	
	for (int i = 0; i < group.size; i++) {
		printf("(%d, %d), ", group.positions[i][0], group.positions[i][1]);
	}
	
	printf("\n");
} 

// from = from - group
Group subtract (Group from, Group group) {
	for (int i = 0; i < group.size; i++) {
		for (int j = 0; j < from.size; j++) {
			if (group.positions[i][0] == from.positions[j][0] 
			&& 	group.positions[i][1] == from.positions[j][1]) {
				
				int last = from.size - 1;
				from.positions[j][0] = from.positions[last][0];
				from.positions[j][1] = from.positions[last][1];
				
				from.size--;
				
				break;
			}
		}
	}
	
	from.mines -= group.mines;
	
	return from;
}

bool cellContainedIn (int x, int y, Group group) {
	for (int i = 0; i < group.size; i++) {
		if (x == group.positions[i][0] && y == group.positions[i][1]) {
			return 1;
		}
	}
	
	return 0;
}

bool containedIn (Group a, Group b) {
	if (a.size > b.size) {
		return 0;
	}
	
	for (int i = 0; i < a.size; i++) {
		if (!cellContainedIn(a.positions[i][0], a.positions[i][1], b)) {
			return 0;
		}
	}
	
	return 1;
}

int intersect(Group a, Group b) {
	int n=0;
	for(int i=0;i<a.size;i++) {
		if (cellContainedIn(a.positions[i][0], a.positions[i][1], b)) {
			n++;
		}
	}
	return n;
}
