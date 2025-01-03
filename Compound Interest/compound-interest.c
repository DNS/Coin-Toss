


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void main () {
	double savings = 100.0;
	double interest_rate = 30.0;	// in % (percent)
	long step = 54;		// each day: 365, each week: 54, each month: 12, each year: 1
	int i;

	printf("%f\n", (savings * (interest_rate / 100 / 1)) + savings);

	for (i=0; i<step; i++) {
		savings = (savings * (interest_rate / 100 / step)) + savings;
	}

	printf("%f\n", savings);
	
	_getch();
}

