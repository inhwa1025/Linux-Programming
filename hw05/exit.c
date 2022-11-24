#include <stdio.h>
#include <stdlib.h>

/* exit.c
 * exit handler를 등록하는 프로그램
 * $ gcc -o exit exit.c (or make exit)
 * $ ./exit
 */

void		// exit handler로 등록할 함수 선언
myexit1()
{
	printf("first exit handler\n");
}

void		// exit handler로 등록할 함수 선언
myexit2()
{
	printf("second exit handler\n");
}

main()
{
	/* Add myexit2 to exit functions */
	if (atexit(myexit2) != 0)  {
		perror("atexit");
		exit(1);
	}

	// myexit1 함수를 exit handler로 등록
	if (atexit(myexit1) != 0)  {
		perror("atexit");
		exit(1);
	}

	// myexit1 함수를 exit handler로 등록
	if (atexit(myexit1) != 0)  {
		perror("atexit");
		exit(1);
	}

	printf("main is done\n");
}
