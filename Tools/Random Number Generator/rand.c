

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

int rand_r (unsigned int *seed) {
	unsigned int next = *seed;
	int result;

	next *= 1103515245;
	next += 12345;
	result = (unsigned int) (next / 65536) % 2048;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (unsigned int) (next / 65536) % 1024;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (unsigned int) (next / 65536) % 1024;

	*seed = next;

	return result;
}

int ex_rand (int start, int end) {
	//srand(time(NULL));
	//return rand() % (end-start) + start;
	UINT32 a = time(NULL);
	return rand_r(&a) % end;
}

void main () {
	printf("%d\n", ex_rand(0, 4));
	getch();
}

