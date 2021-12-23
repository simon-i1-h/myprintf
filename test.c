#include <limits.h>
#include <stdio.h>

#include "myprintf.h"

int
main(void)
{
	int r = myprintf("hello.\n");
	printf("r: %d\n", r);

	myprintf("percent: %%\n");

	myprintf("answer: %d\n", 0);
	myprintf("answer: %d\n", 42);
	myprintf("answer: %d\n", INT_MAX);

	myprintf("char: %c\n", 'a');
	myprintf("char: %c\n", '\n');
	myprintf("char: %c\n", '!');

	return 0;
}
