﻿
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>


void main () {
	int x, i;

	srand(time(NULL));

	for (i=0; i<100; i++) {
	x = rand() %	 ( RAND_MAX / 1 + 1 ) ;
	printf("%d\n", x);
	}

	system("pause");
}

