
/*
	random-market-test.c

	Test the market history prices using random buy/sell trading strategy.

	Result: Forex prices follow Geometric Brownian Motion.
*/


#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#include "csvparser.h"



void main () {
	int i, x;
	BOOL trade_status = FALSE;
	double open_trade;
	long win = 0, loss = 0;
	CsvParser *csvparser;
	CsvRow *header;
	CsvRow *row;
	char **headerFields;

	srand(time(NULL));

	csvparser = CsvParser_new("EURUSD.csv", ",", 1);

	header = CsvParser_getHeader(csvparser);
	if (header == NULL) {
		printf("%s\n", CsvParser_getErrorMessage(csvparser));
		return 1;
	}
	
	headerFields = CsvParser_getFields(header);
	/*for (i=0; i<CsvParser_getNumFields(header); i++) {
		printf("TITLE: %s\n", headerFields[i]);
	}*/

	


	
	while ((row = CsvParser_getRow(csvparser)) ) {
		char **rowFields = CsvParser_getFields(row);
		
		//for (i=0 ; i<CsvParser_getNumFields(row); i++) {
			//printf("FIELD 1 : %f\n", atof(rowFields[1]));
			

		if (trade_status == FALSE) {
		
			x = rand() % 2;
			open_trade = atof(rowFields[1]);
			trade_status = TRUE;
		} else {
			if (x == 1) {
				if (atof(rowFields[1]) >= (open_trade + 0.0050f)) {
					trade_status = FALSE;
					win++;
				} 
				if (atof(rowFields[1]) <= (open_trade - 0.0050f)) {
					trade_status = FALSE;
					loss++;
				}
			} else if (x == 0) {
				if (atof(rowFields[1]) <= (open_trade - 0.0050f)) {
					trade_status = FALSE;
					win++;
				} else if (atof(rowFields[1]) >= (open_trade + 0.0050f)) {
					trade_status = FALSE;
					loss++;
				}
			}
		}

		//}

	CsvParser_destroy_row(row);

	}

	

	printf("win: %d\n", win);
	printf("loss: %d\n", loss);



	CsvParser_destroy(csvparser);

	getch();
}

