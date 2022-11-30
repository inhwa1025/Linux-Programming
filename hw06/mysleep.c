#include <stdio.h>
#include <signal.h>

/* mysleep.c
 * sleep systemcall을 구현한 프로그램
 * SIGALRM signal handler를 등록하고 nsecs 후에 자신에게 SIGALRM 시그널을 보냄
 * $ gcc -o mysleep mysleep.c (or make mysleep)
 * $ ./mysleep
 */
void
SigAlarmHandler(int signo)
{
	/* nothing to do, just return to wake up the pause */
	return;
}

unsigned int
mysleep(unsigned int nsecs)
{
	/* SIGALRM signal handler: SigAlarmHandler */
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		return nsecs;
	}

	// alarm(nsecs): nsecs 후에 SIGALRM 시그널을 발생시키는 systemcall
	alarm(nsecs);

	// signal 발생을 기다림
	pause();

	// 이전 alarm으로부터 얼마나 시간이 남았는지 return
	// 다른 시그널로 인해 pause가 종료된 것이 아닌 의도대로 종료되었다면 0 return
	return alarm(0);
}

main()
{
	printf("Wait for 5 seconds...\n");

	mysleep(5);
}
