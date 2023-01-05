#include <stdio.h>
#include <string.h>

void main (int argc, char *argv[])
{
	char* dst = argv[1];
	char ch = argv[2][0];
	char result[sizeof(argv[1])];
	char* res = result;

	while (*dst)  {
		//printf("%c\n", *dst);
		if (*dst != ch) {
			*res++ = *dst;
		}
		*dst++;
	}
	if (*dst != ch) {
		*res = *dst;
	}
	printf("%s\n", result);
}
