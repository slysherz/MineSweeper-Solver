void printBoard(Board board) {
	puts("\n");
	
	for (int y = 0; y < board.height; y++) {
		for (int x = 0; x < board.width; x++) {
			printf("%c", get(board, x, y));
		}
		printf("\n");
	}
}

bool checkBounds (Board board, int x, int y) {
	return x >= 0
		&& x < board.width
		&& y >= 0 
		&& y < board.height;
}

char get (Board board, int x, int y) {
	if (!checkBounds(board, x, y)) {
		return OUT_OF_BOUNDS;
	}
	
	return board.grid[x + y * board.width];
}

void set (Board board, int x, int y, char value) {
	if (!checkBounds(board, x, y)) {
		return;
	}
	
	board.grid[x + y * board.width] = value;
}

int size(Board board) {
	return board.width * board.height;
}

Board buildBoardFromFile (char *file) {
	printf("Building board from file: %s\n", file);
	
	FILE *filePtr;
	if ((filePtr = fopen(file, "r")) == NULL)
	{
		printf("Problem, cannot open %s for reading.\n", file);
		exit(0);
	}
	else {
		char buff[GSIZE];
		int width = 0;
		int height = 0;
		while (fgets(buff, GSIZE, filePtr) != NULL)
		{
			for (int x = 0; x < strlen(buff); x++) {
				if(buff[x] == '\n' || buff[x] == '\r') {
					buff[x] = '\0';
				}
			}
			
			width = MAX(width, strlen(buff));
			height++;
		}
		
		// Create array
		Board board;
		board.width = width;
		board.height = height;
		char grid[size(board)];	// BUG? Doesn't this go out of scope?
		memset(grid, OUT_OF_BOUNDS, sizeof(grid));
		board.grid = &grid[0];
		
		rewind(filePtr);
		
		printf("Width: %d, Height: %d\n", width, height);
		for (int y = 0; y < height; y++) {
			fgets(buff, GSIZE, filePtr);
			
			for (int x = 0; x < strlen(buff); x++) {
				if(buff[x] == '\n' || buff[x] == '\r') {
					break;
				}
				
				set(board, x, y, buff[x]);
			}
		}
		fclose(filePtr);
		
		return board;
    }
}
