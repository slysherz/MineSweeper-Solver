#include <stdlib.h>
#include "ui.h"

void getMineSweeperInput (int *x, int *y, char *played) {
	char separator[17];
	
	puts("\nInput: C|M|? X Y");
	while (1) {
		char *line = NULL;
		size_t len = 0;
		if (getline(&line, &len, stdin) == -1) continue;
		
		*x = -1;
		*y = -1;
		*played = 0;
		int scanned = sscanf(line, "%c%16[^0-9]%d%16[^0-9]%d", played, separator, x, separator, y);
		free(line);
		
		if (scanned > 0 && *played != 0 && *x != -1 && *y != -1) {
			// Check that parameters make sense
			printf("\n[%d] Got: %c at (%d, %d)", scanned, *played, *x, *y);
			break;
		}
		
		puts("Invalid intput, please try again: ");
	}
}
