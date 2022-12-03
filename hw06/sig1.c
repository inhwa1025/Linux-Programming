#include <stdio.h>
#include <signal.h>

/* sig1.c
 * SIGINT 시그널의 signal handler로 SigIntHandler를 등록하는 프로그램
 * $ gcc -o sig1 sig1.c (or make sig1)
 * $ ./sig1
 * ^C
 */
void
SigIntHandler(int signo)
{
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");

	exit(0);
}

main()
{
	/* SIGINT signal handler: SigIntHandler */
	signal(SIGINT, SigIntHandler);

	printf("Press ^C to quit\n");

	for ( ; ; )
		pause();	// signal 발생을 기다림
}
