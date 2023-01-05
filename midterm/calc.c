#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int a = atoi(argv[1]);
	int b = atoi(argv[argc-1]);

	if (strcmp(argv[2], "+") == 0) {
		printf("%d\n", a + b);
	}
	else if (strcmp(argv[2], "-") == 0) {
        printf("%d\n", a - b);
	}
	else if (strcmp(argv[2], "*") == 0) {
	    printf("%d\n", a * b);
	}
	else if (strcmp(argv[2], "/") == 0) {
	    printf("%d\n", a / b);
	}
	else {
		printf("%d\n", a * b);
	}
	
	return 0;
}
