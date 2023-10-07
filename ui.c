#include "ui.h"

void getMineSweeperInput (int *x, int *y, char *played) {
	char separator[16];
	
	puts("\nInput:");
	while (1) {
		
		int scanned = scanf("%c%16[^0-9]%d%16[^0-9]%d", played, separator, x, separator, y);
		fseek(stdin, 0, SEEK_END);	// Flush new line
		
		if (scanned > 0) {
			// Check that parameters make sense
			printf("\nGot: %c at (%d, %d)", *played, *x, *y);
			break;
		}
		
		puts("Invalid intput, please try again: ");
	}
}
