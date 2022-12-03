#include <stdio.h>
#include <signal.h>

/* sig2.c
 * SIGUSR1, SIGUSR2 시그널의 signal handler로 SigUsrHandler를 등록하는 프로그램
 * $ gcc -o sig2 sig2.c (or make sig2)
 * $ ./sig2
 * $ ps -ef | grep lsp63
 * $ kill -USR1 <processid>
 * $ kill -USR2 <processid>
 * $ kill -TERM <processid>
 */
void
SigUsrHandler(int signo)
{
	if (signo == SIGUSR1)  {	// SIGUSR1 시그널 발생시 아래를 출력
		printf("Received a SIGUSR1 signal\n");
	}
	else if (signo == SIGUSR2)  {	// SIGUSR2 시그널 발생시 아래를 출력
		printf("Received a SIGUSR2 signal\n");
	}
	else  {		// 그 외 시그널 발생시 아래를 출력하고 프로세스 종료
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

main()
{
	/* SIGUSR1 signal handler: SigUsrHandler */
	if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	// SIGUSR2 시그널의 signal handler 등록
	if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	for ( ; ; )
		pause();	// 시그널 발생을 기다림
}
