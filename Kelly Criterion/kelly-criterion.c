
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

/*
	f* = (bp-q)/b = p(b+1)-1 / b

	f* is the fraction of the current bankroll to wager;
	b is the net odds received on the wager ("b to 1"); that is, you could win $b (and get a return of your $1 wagered) for a $1 bet
	p is the probability of winning;
	q is the probability of losing, which is 1 - p.
*/

#define ODDS 1.0f
#define PROBABILITY	0.51f

float kelly (float b, float p) {
	float f;
	f = (p*(b+1)-1) / b;
	return f;
}

float gamble () {
	int i;
	float wager = 10.0f;	// start with $1000
	int flip_coin;
	float bet;
	
	float f = kelly(ODDS, PROBABILITY);
	//printf("< KELLY: %f%%\n", f*100);
	
	for (i=0; i<10000; i++) {
		if (wager <= 1.0f) {
			//puts("<<<<< BANKRUPT >>>>>");
			break;
		}
		
		bet = wager * 0.1;//wager * f;
		
		
		flip_coin = rand() % 10;
		//printf("flip_coin: %d\n", flip_coin);
		if (flip_coin < (PROBABILITY * 10)) {
			// win
			wager += bet * ODDS;
		} else {
			// lose
			wager -= bet * ODDS;
		}
		
		
	}
	
	//printf("%.2f\n", wager);
	return wager;
}

void main () {
	int i;
	int person_win = 0;
	int person_loss = 0;
	
	srand(time(NULL));
	
	for (i=0; i<1000; i++) {
		float ret = gamble();
		if (ret <= 10.0f)
			person_loss++;
		else
			person_win++;
		
	}
	
	printf("W: %d\n", person_win);
	printf("L: %d\n", person_loss);
	
	
	printf("\nPress any key to exit . . .");
	getch();
}

