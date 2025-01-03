
/*
    File Name   : prospect-debunk.c
    Description : Monte Carlo simulator to show the Prospect Theory is wrong.
    Author      : Daniel Sirait <dsirait@aol.com>
    Copyright   : Copyright (c) 2014 Daniel Sirait
    License     : Proprietary
    Disclaimer  : I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
                  ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS PROGRAM,
                  WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>


int REPEAT_THROW = 1000000;

void main () {
	int x, i, count = 0, total = 0;
	char c;
	srand(time(NULL));

	for (i = 0; i<REPEAT_THROW; i++) {
		x = rand() % 1000;
		count++;
		if (x == 400) {
			printf("match at: %d\n", count);
			count = 0;
			total++;
		}
	}

	printf("\n");
	printf("expected current_tossed match : %d\n", REPEAT_THROW / 1000);
	printf("current random match          : %d\n", total);
	printf("Probability (Result/Total Events) = %.15f\n", 1000 / REPEAT_THROW);

	
	printf("\nPress any key to exit . . .");
	getch();
}