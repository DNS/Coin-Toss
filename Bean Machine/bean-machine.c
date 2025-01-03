
/* 
Project		: Bean Machine
Status		: Complete
Author		: Daniel Sirait <dsirait@outlook.com>
Copyright	: Copyright (c) 2013 Daniel Sirait
License		: Must be distributed only under the terms of "THE BEER-WARE LICENSE"(Revision 42):
              As long as you retain this notice you
              can do whatever you want with this stuff. If we meet some day, and you think
              this stuff is worth it, you can buy me a beer in return
Disclaimer	: I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
              ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS DOCUMENT,
              WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
Description	: The bean machine, to demonstrate the central limit theorem, in particular 
              that the normal distribution is approximate to the binomial distribution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

typedef enum {
	LEFT,
	RIGHT
} DIRECTION;


DIRECTION bean_hit () {
	int r = rand() % 2;
	//printf ("-> %d\n", r);
	if (r == 0) return LEFT;
	else if (r == 1) return RIGHT;
}

void main () {
	int i, j;
	float pos;
	int distribution[9] = {0,0,0,0, 0 ,0,0,0,0};
	
	srand (time(NULL));

	for (i=0; i<100000; i++) {
		pos = 4.0f;		// bean drop position
		for (j=0; j<8; j++) {
			DIRECTION d = bean_hit();
			if (d == LEFT) pos -= 0.5f;
			else if (d == RIGHT) pos += 0.5f;
		}
		distribution[(int) pos] += 1;
	}
	
	for (i=0; i<9; i++) {
		printf ("distribution[%d]: %d\n", i, distribution[i]);
	}
	
	printf("\nPress any key to exit . . .");
	getch();
}





