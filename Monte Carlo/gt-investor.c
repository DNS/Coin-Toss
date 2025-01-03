
/*
    File Name   : gt-investor.c
    Description : Economic Game Theory (Monte Carlo simulator)
    Author      : Daniel Sirait <dsirait@outlook.com>
    Copyright   : Copyright (c) 2014 Daniel Sirait
    License     : Proprietary
    Disclaimer  : I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
                  ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS PROGRAM,
                  WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <windows.h>


#define NUMBER_OF_TRADES 50000
#define WAGER 1000.f
#define NUMBER_OF_INVESTOR 1000
#define INITIAL_INVESTOR_MONEY 10000.f


typedef enum _COINTOSS {
	HEAD,
	TAIL
} COINTOSS;

typedef struct _INVESTOR {
	float money;
	COINTOSS guess;		// 0: head, 1: tail
} INVESTOR;


COINTOSS toss_coin () {
	int x = rand() % 2;
	if (x == 0) return HEAD;
	else if (x == 1) return TAIL;
}


void trade (INVESTOR *investor) {
	COINTOSS outcome;
	int winner_count = 0, loser_count = 0;
	float sum_money = 0.f, money_given = 0.f;
	int i;

	outcome = toss_coin();

	for (i=0; i<NUMBER_OF_INVESTOR; i++) {
		if (investor[i].money <= 0.f) continue;		// bankrupt, stop loser from trading
		investor[i].guess = toss_coin();
	}


	// count winner & loser
	for (i=0; i<NUMBER_OF_INVESTOR; i++) {
		if (investor[i].money <= 0.f) continue;		// stop loser from trading
		if (investor[i].guess == outcome) winner_count++;
		else loser_count++;
	}
	
	sum_money = loser_count * WAGER;			// give money to the winner, bet is [WAGER]
	money_given = sum_money / winner_count;

	// give money to winner & take money from loser
	for (i=0; i<NUMBER_OF_INVESTOR; i++) {
		if (investor[i].money <= 0.f) continue;		// skip loser
		if (investor[i].guess == outcome) {
			investor[i].money += money_given;
		} else {
			investor[i].money -= WAGER;				// bet is [WAGER]
		}
	}


}


void print_result (INVESTOR *investor) {
	int i, winner_count = 0, loser_count = 0, bankrupt_count = 0;

	// count ALL winner & loser
	for (i=0; i<NUMBER_OF_INVESTOR; i++) {
		if (investor[i].money >= INITIAL_INVESTOR_MONEY) winner_count++;
		else if (investor[i].money < INITIAL_INVESTOR_MONEY) {
			loser_count++;
			if (investor[i].money <= 0.f)	 bankrupt_count++;
		}
	}

	printf("INITIAL_INVESTOR_MONEY: %.0f\n", INITIAL_INVESTOR_MONEY);
	printf("WAGER: %.0f\n", WAGER);
	printf("NUMBER_OF_TRADES: %d\n", NUMBER_OF_TRADES);
	printf("NUMBER_OF_INVESTOR: %d\n\n", NUMBER_OF_INVESTOR);

	printf("winner_count: %d\n", winner_count);
	printf("loser_count: %d\n", loser_count);
	printf("bankrupt_count: %d\n\n\n", bankrupt_count);

	puts("RESULT");
	puts("------");
	printf("> Richer	  : %.2f%%\n", (float) winner_count / NUMBER_OF_INVESTOR * 100);
	printf("> Poorer	  : %.2f%%\n\n", (float) loser_count / NUMBER_OF_INVESTOR	 * 100);

	printf("> Bankrupt : %.2f%%\n", (float) bankrupt_count / NUMBER_OF_INVESTOR	 * 100);
}


void main () {
	INVESTOR investor[NUMBER_OF_INVESTOR];				// number of people doing economic activities
	int i;
	//puts("123");
	//fflush(stdout);
	srand(time(NULL));

	for (i=0; i<NUMBER_OF_INVESTOR; i++) 
		investor[i].money = INITIAL_INVESTOR_MONEY;		// give each investor money [INITIAL_INVESTOR_MONEY]

	for (i=0; i<NUMBER_OF_TRADES; i++) 
		trade(investor);
	
	print_result(investor);

	printf("\nPress any key to exit . . .");
	getch();
}



