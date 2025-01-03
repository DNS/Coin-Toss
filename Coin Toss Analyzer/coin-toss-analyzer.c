
/* 
Project    : Coin Toss Analyzer
Author     : Daniel Sirait <dns@cpan.org>
Copyright  : Copyright (c) 2013 Daniel Sirait
License    : Must be distributed only under the terms of "THE BEER-WARE LICENSE" (Revision 42):
             As long as you retain this notice you
             can do whatever you want with this stuff. If we meet some day, and you think
             this stuff is worth it, you can buy me a beer in return
Disclaimer : I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
             ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS DOCUMENT,
             WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>


typedef enum {
	HEAD,
	TAIL
} RESULT;


RESULT toss_coin () {
	int r = rand() % 2;
	if (r == 0) return HEAD;
	else if (r == 1) return TAIL;
}

void main () {
	int i, j;
	int min_dev = 0, max_dev = 0, abs_dev = 0, data = 0;
	int total_sample = 15;	// check for repeated same result

	srand(time(NULL));

	for (i=0; i<90000000; i++) {
		data = 0;
		min_dev = 0;
		max_dev = 0;
		abs_dev = 0;
		for (j=0; j<total_sample; j++) {
			RESULT r = toss_coin();
			if (r == HEAD) data += 1;
			else if (r == TAIL) data -= 1;
			if (data > max_dev) max_dev = data;
			if (data < max_dev) min_dev = data;
			if (max_dev >= abs(min_dev)) abs_dev = max_dev;
			if (max_dev <= abs(min_dev)) abs_dev = abs(min_dev);
		}
		printf("abs_dev_percent: %f%%\n", (float) abs_dev / total_sample * 100.f);
		if (max_dev == total_sample || abs(min_dev) == total_sample) {
			printf("BINGO: abs_dev is ZERO !!!\n");
			
			break;
		}
		//fflush(stdout);
	}
	
	
	printf("\nPress any key to exit . . .");
	getch();
}


