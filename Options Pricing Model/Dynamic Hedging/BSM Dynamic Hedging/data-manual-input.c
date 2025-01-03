
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "bsm.h"



__inline double round (double x) {
	if (x >= 0.0) return ((floor(x)+0.5) <= x) ? ceil(x) : floor(x);
	else return ((ceil(x)-0.5) < x) ? ceil(x) : floor(x);
}

void main () {
	BSM val;
	double portfolio = 20000.0;
	int option = 0;		// multiply by 100
	int stock = 0;
	double v = 0.50;
	double S = 100.0;
	double K = 100.0;
	int expiration = 3;
	double t;
	double r = 0.05;

	//double commission = 1.0;

	/*double prices[] = {100.0, 103.0, 98.0, 97.0, 96.0, 99.0, 101.0, 102.0, 103.0, 104.0, 106.0, 107.0, 105.0, 104.0, 102.0, 
						101.0, 97.0, 98.0, 94.0, 92.0, 96.0, 95.0, 94.0, 96.0, 98.0, 100.0, 101.0, 99.0, 102.0, 100.0};*/
	double prices[] = {100.0, 102.6, 100.0, 103.0};

	//double x = bsm_volatility(S, K, r, t, 6.023253656682620, 5.613138030053726);

	int i, len = sizeof(prices)/sizeof(double);
	expiration = len;

	for (i=0; i<len; i++) {
		//double option_value = option * 100 * val.put;
		S = prices[i];
		t = expiration / 365.0;
		val = bsm(S, K, r, t, v);
		expiration--;
		//printf("%d: %f\n", i, val.put);

		if (option <= 0) {
			// init
			option = round((10000.0/S) / fabs(val.delta_put) / 100);
			portfolio -= option * 100 * val.put;

			stock = round(option * 100 * fabs(val.delta_put));		//
			portfolio -= stock * S;
			
			printf("traded portfolio: %f\n\n", portfolio);
		} else {
			// dynamic hedge
			if (i >= len-1) {
				// last day
				/*printf("\nportfolio: %f\n", portfolio);
				printf("option: %d\n", option);
				printf("stock: %d\n", stock);
				printf("S: %f\n", S);*/
				portfolio += (option * 100 * val.put);
				portfolio += (stock * S);
				option = 0;
				stock = 0;
				break;
			} else {
				int delta_hedge = round((option * 100 * val.delta_put) + stock);
				//printf("> %f\n", (option * 100 * val.delta_put) + stock);
				printf(">> %d\n", delta_hedge);
				
				if (delta_hedge > 0) {
					//int sell_stock = round((delta_hedge) / S);
					//printf("sell_stock: %d\n", delta_hedge);
					stock -= abs(delta_hedge);
					portfolio += abs(delta_hedge) * S;
					//printf(">stock a: %d\n", stock);
				} else if (delta_hedge < 0) {
				//	int buy_stock = round(fabs(delta_hedge) / S);
				//	//printf("buy_stock: %d\n", buy_stock);
					stock += abs(delta_hedge);
					portfolio -= abs(delta_hedge) * S;
				//	//printf("> %f\n", 1 * val.delta_put);
					//printf(">stock b: %d\n", stock);
				}

				printf("%d - (%.1f): %f\n", i, S, portfolio + (stock * S) + (option * 100 * val.put));
			}
		}

	}

	printf("\n> %f\n", portfolio);


	//printf("%d\n", sizeof(prices)/8);

	/*val = bsm(S, K, r, t, v);

	printf("Call	 	 	  : %.15f\n", val.call);
	printf("Put	 	 	 	 : %.15f\n", val.put);

	printf("Delta Call : %.15f\n", val.delta_call);
	printf("Delta Put	 : %.15f\n", val.delta_put);
	printf("Gamma	 	 	 : %.15f\n", val.gamma);
	printf("Kappa	 	 	 : %.15f\n", val.kappa);
	printf("Theta Call : %1.15f\n", val.theta_call);
	printf("Theta Put	 : %.15f\n", val.theta_put);
	printf("Rho Call	  : %.15f\n", val.rho_call);
	printf("Rho Put	 	 : %.15f\n", val.rho_put);*/







	printf("\nPress any key to exit . . .");
	getch();
}

