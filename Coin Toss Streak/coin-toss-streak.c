
/* 
Project     : Coin Toss Streak
Author      : Daniel Sirait <dns@cpan.org>
Copyright   : Copyright (c) 2013 Daniel Sirait
License     : Must be distributed only under the terms of "THE BEER-WARE LICENSE"	 (Revision 42):
              As long as you retain this notice you
              can do whatever you want with this stuff. If we meet some day, and you think
              this stuff is worth it, you can buy me a beer in return
Disclaimer  : I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
              ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS DOCUMENT,
              WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

void main () {
	int i, lim = 100000000;
	int head = 0, tail = 0;
	int hstreak = 0, tstreak = 0;
	int hstreakmax = 0, tstreakmax = 0;

	srand(time(NULL));

	for (i = 0; i < lim; i++) {
		//if (rand()%2)
		if (rand() & (1<<13) ) {			//pick a bit, try different bits
			head++;
			if (++hstreak > hstreakmax) hstreakmax=hstreak;
			tstreak=0;
		}
		else {
			tail++;
			if (++tstreak > tstreakmax) tstreakmax=tstreak;
			hstreak=0;
		}
	}
	printf("Ran %d times\n",lim);
	printf("head %d, streak %d\n",head,hstreakmax);
	printf("tail %d, streak %d\n",tail,tstreakmax);
	
	printf("\nPress any key to exit . . .");
	getch();
}


