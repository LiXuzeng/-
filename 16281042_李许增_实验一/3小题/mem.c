#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	int *p = malloc(sizeof(int)); // a1
	assert(p != NULL);
	printf("(%d) address pointed to by p: %p\n",getpid(), p); // a2
	*p = 0; // a3
	while (1) {
		sleep(2);
		*p = *p + 1;
		printf("(%d) p: %d\n", getpid(), *p); // a4
	}
	return 0;
}
