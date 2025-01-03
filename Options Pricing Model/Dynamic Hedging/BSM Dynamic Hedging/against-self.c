

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "bsm.h"
#include "csvparser.h"

//
//void main () {
//	int i, x;
//	 	 CsvParser *csvparser;
//	 	 CsvRow *header;
//	 	 CsvRow *row;
//	char **headerFields;
//	BSM bs;
//	int total_trade = 0;
//	int expiry = 60;
//	BOOL is_first = FALSE;
//
//	double portfolio = 1000000.0;
//	double S;
//	double K = 100.0;
//	double r = 0.02;
//	double t;
//	double v = 0.25;
//	double cost = 0.0;
//	int put_total = 0, shares = 0;
//	const double TCOST = 10.0;
//	
//
//	printf("%f\n", portfolio);
//
//	csvparser = CsvParser_new("stock prices\\IBM.csv", ",", 1);
//
//	 	 header = CsvParser_getHeader(csvparser);
//	 	 if (header == NULL) {
//	 	 	 	 printf("%s\n", CsvParser_getErrorMessage(csvparser));
//	 	 	 	 return;
//	 	 }
//	 	 
//	headerFields = CsvParser_getFields(header);
//
//
//	
//
//	while ((row = CsvParser_getRow(csvparser)) ) {
//		char **rowFields = CsvParser_getFields(row);
//
//		S = atof(rowFields[1]);
//		t = (double) expiry / 365;
//		bs = bsm(S, K, r, t, v);
//
//
//		if (expiry <= 3) {
//			
//			expiry = 60;
//			portfolio += put_total * 100 * bs.put - TCOST;
//			portfolio += shares * S - TCOST;
//			put_total = 0;
//			shares = 0;
//		} else {
//			
//			if (expiry >= 60) {
//				// buy 2 put contract (200 option unit) @ ~ 100
//				put_total = 2;
//				shares = (int) 2 * 100 * fabs(bs.delta_put);
//				portfolio -= put_total * 100 * bs.put + TCOST;		// option value + transaction cost
//				portfolio -= shares * S + TCOST;
//			} else {
//				int hedge_shares = (int) 2 * 100 * fabs(bs.delta_put);		// BUG: hedge_shares
//				printf("hedge_shares: %d\n", hedge_shares);
//
//				if (hedge_shares > shares) {
//					portfolio -= (hedge_shares - shares) * S + TCOST;
//					shares = hedge_shares;
//					
//				} else if (hedge_shares < shares) {
//					portfolio += (shares - hedge_shares) * S - TCOST;
//					shares = hedge_shares;
//				}
//
//			}
//		}
//
//		expiry--;
//		CsvParser_destroy_row(row);
//	}
//
//
//	if (put_total >= 0)
//		portfolio += put_total * 100 * bs.put - TCOST;
//
//	if (shares >= 0)
//		portfolio += shares * S - TCOST;
//
//
//	printf("%f\n", portfolio);
//	puts("end");
//
//
//	CsvParser_destroy(csvparser);
//
//
//	getch();
//}
//




__inline double round (double x) {
	if (x >= 0.0) return ((floor(x)+0.5) <= x) ? ceil(x) : floor(x);
	else return ((ceil(x)-0.5) < x) ? ceil(x) : floor(x);
}

void main () {
	BSM val;
	double portfolio = 10000.0;
	int option = 0;		// initial 0 contract, but multiply by 100
	int stock = 0;		// initial 0 share
	double v = 0.25;
	double S = 100.0;
	double K = 100.0;
	int expiration = 60;
	double t;
	double r = 0.00;

	//double commission = 1.0;

	
	//double prices[] = {37.35,37.2,36.85,36.33,36,35.88,35.9,35.99,34.73,35.9,36.69,36.83,36.82,36.26,36.09,37.45,36.87,36.12,35.98,36.79,36.95,37.74,36.97,36.29,35.8,36.32,36.63,36.88,37.35,37.33,37.39,37.63,37.22,37.57,37.94,37.69,37.61,37.58,37.45,37.98,37.92,38.2,38.25,38.14,38.28,37.99,37.87,37.8,38.42,37.65,37.9,38.26,39.47,39.25,40.72,40.34,40.66,40.48,39.74,39.79,40.43,41.15,41.44,41.29,41.25,39.96,39.75,39.93,40.44,39,39.11,39.34,40.06,40.01,40.13,39.96,39.99,39.74,40.29,40.14,41.1,40.4,40.24,40.31,39.52,39.29,39.22,39.34,39.54,39.74,39.92,40.3,40.09,39.67,39.61,39.68,39.8,40.29,40.37,40.26,40.14,40.15,40.45,40.95,40.6,40.21,40.59,41.48,41.39,41.03,40.93,40.81,41.1,41.04,41.29,41.61,41.57,41.45,41.73,41.83,41.7,41.93,41.61,42.17,41.86,41.73,41.91,41.75,41.87,41.98,41.37,41.7,42.22,42.33,42.51,45.45,44.65,44.56,45,45.45,44.93,44.3,44.36,43.91,44.07,43.38,43.21,42.97,43.31,42.74,42.84,43.23,43.26,43.04,43.68,44.08,44.58,44.94,44.97,45.34,44.84,45.35,45.4,45.31,44.9,44.75,45.09,45.43,44.53,44.74,45.11,46.02,46.47,46.82,46.74,46.91,46.54,46.39,46.26,46.59,46.81,47.3,46.85,46.63,46.88,45.93,45.98,46.37,46.27,45.83,45.98,46.12,45.86,45.48,46.5,45.6,43.82,43.87,43,42.53,43.2,43.06,44.36,45,44.62,46.83,45.71,45.86,46.44,46.32,46.94,46.89,47.3,47.8,47.86,48.92,48.65,48.85,48.56,48.81,49.74,49.41,49.13,48.66,48,49.02,47.99,47.66,47.49,47.95,47.88,48.84,48.44,48.39,48.82};
	double prices[] = {100.0, 100.0, 105.0};

	//double x = bsm_volatility(S, K, r, t, 6.023253656682620, 5.613138030053726);

	int i, len = sizeof(prices)/sizeof(double);
	//len = 3;

	for (i=0; i<len; i++) {
		//double option_value = option * 100 * val.put;
		S = prices[i];
		t = expiration / 365.0;
		val = bsm(S, K, r, t, v);
		expiration--;
		//printf("%d: %f\n", i, val.put);

		if (option <= 0) {
			// init
			//option = 1;		//round((10000.0/S) / fabs(val.delta_put) / 100);
			option = round((10000.0/S) / fabs(val.delta_put) / 100);
			portfolio -= option * 100 * val.put;

			stock = round(option * 100 * fabs(val.delta_put));
			portfolio -= stock * S;
			
			printf("traded stock shares: %d @ 100 = %f\n", stock, stock * 100.0);
			printf("traded option: %f x %d x 100 = %f\n", val.put, option, option * 100 * val.put);
			printf("traded portfolio: %f\n\n", 10000.0 - portfolio);
			//Sleep(50000);
		} else {
			
			if (i >= len-1) {
				// last day (extract option & stock value to portfolio)
				//printf("\nportfolio: %f\n", portfolio);
				//printf("option: %d\n", option);
				//printf("stock: %d\n", stock);
				//printf("S: %f\n", S);
				portfolio += option * 100 * val.put;
				portfolio += stock * S;
				option = 0;
				stock = 0;
				printf(">> %d\n", 0);
				printf("hedge: %d - %.1f - %d: %f\n", i, S, stock, portfolio + (stock * S) + (option * 100 * val.put));

				printf("\nportfolio: %f\n", portfolio);

				//break;
			} else {
				// dynamic hedge
				int delta_hedge = round((option * 100 * val.delta_put) + stock);
				printf(">> %d\n", delta_hedge);
				if (delta_hedge > 0) {
					stock -= abs(delta_hedge);
					portfolio += abs(delta_hedge) * S;
				} else if (delta_hedge < 0) {
					stock += abs(delta_hedge);
					portfolio -= abs(delta_hedge) * S;
				}

				printf("hedge: %d - %.1f - %d: %f\n", i, S, stock, portfolio + (stock * S) + (option * 100 * val.put));
			}
		}

	}

	printf("\nlen: %d", len);
	printf("\ntotal portfolio: %f\n", portfolio);


	//printf("%d\n", sizeof(prices)/8);


	

	printf("\nPress any key to exit . . .");
	getch();
}


//void main () {
//	BSM val;
//	double v = 0.25;
//	double S = 100.0;
//	double K = 100.0;
//	double t = 90/365.0;
//	double r = 0.00;
//
//
//	val = bsm(S, K, r, 90/365.0, v);
//	printf("%f\n", val.call);
//	val = bsm(S, K, r, 88/365.0, v);
//	printf("%f\n", val.call);
//
//	getch();
//}

