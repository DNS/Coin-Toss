
/*
    File Name    : bernoulli-trial.c
    Author       : Daniel Sirait <dsirait@aol.com>
    Copyright    : Copyright (c) 2014 Daniel Sirait
    License      : Proprietary
    Disclaimer   : I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
                   ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS PROGRAM,
                   WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
    Description  : To demonstrate the central limit theorem, in particular 
                   that the binomial distribution (bernoulli trial) is 
                   approximate to the normal distribution.
    Known Bugs   : There's a minor bug on MSVCRT pseudo-random generator.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "mt19937int.h"


#define TOTAL_THROW 10000000


typedef enum _COINTOSS {
	UNDEFINED = 0,
	HEAD = 1,
	TAIL = 2
} COINTOSS;




COINTOSS toss_coin () {
	int x = genrand() % 2;
	//int x = rand() % 2;
	if (x == 0) return HEAD;
	else if (x == 1) return TAIL;
}

void main () {
	int i = 0, j, repeat = 0;
	int distribution[51] = {0};
	int count = 0;
	signed int v = 0;
	int pos = 25;		// central limit theorem, mean probability
	COINTOSS previos_toss = UNDEFINED;
	COINTOSS current_toss;
	sgenrand(time(NULL));
	srand(time(NULL));

	while (i<TOTAL_THROW) {
		current_toss = toss_coin();			 // current toss

		if (previos_toss == UNDEFINED) {
			repeat = 0;
			previos_toss = current_toss;
		} else if (previos_toss != UNDEFINED) {
			if (previos_toss == current_toss) {
				repeat += 1;
			} else {
				if (previos_toss == HEAD) 
					distribution[pos+repeat] += 1;
				if (previos_toss == TAIL) 
					distribution[pos-repeat] += 1;

				previos_toss = current_toss;
				repeat = 0;
			}
		}

		if (i == TOTAL_THROW-1) {
			if (previos_toss != current_toss) {
				previos_toss = current_toss;
				repeat = 0;
			}

			if (previos_toss == HEAD) 
				distribution[pos+repeat] += 1;
			if (previos_toss == TAIL) 
				distribution[pos-repeat] += 1;
		}

		i++;
	}


	j = 26;
	for (i=0; i<25; i++) {
		printf("%d :\t%d\n", i-25, distribution[i]);
		count += (distribution[i] * j);
		j--;
	}

	for (i=25; i<51; i++) {
		printf("+%d :\t%d\n", i-25, distribution[i]);
		count += distribution[i] + (distribution[i] * (i-25));
	}

	printf("\nTOTAL_THROW: %d\n", TOTAL_THROW);



	printf("Verify Total Throw: %d\n", count);

	printf("\nPress any key to exit . . .");
	getch();
}

