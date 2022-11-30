#include <stdio.h>
#include <signal.h>

/* alarm.c
 * 1초마다 alarm을 반복하여 signal handler로 등록된 일을 주기적으로 수행하도록 한 프로그램
 * $ gcc -o alarm alarm.c (or make alarm)
 * $ ./alarm
 */

// 전역변수를 사용하지 않는 것이 원칙이지만 어쩔 수 없는 경우
// static을 사용하여 해당 파일 외에 다른 파일에서 사용하지 못하도록 한다.
static unsigned int	AlarmSecs;


void
SigAlarmHandler(int signo)
{
	// signal handler를 다시 등록
	// Solaris에서는 handler를 매번 등록해줘야하지만 Linux에서는 재등록 필요 없음
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	// 전역변수에 설정된 값을 사용하여 alarm
	alarm(AlarmSecs);

	/* Do something */
	printf(".");
	// OS는 \n이 나올때까지 모았다가 한 번에 출력함
	// 매번 출력되도록 하기위해 fflush를 사용해 stdout의 버퍼캐시를 비운다.
	fflush(stdout);

	return;
}

int
SetPeriodicAlarm(unsigned int nsecs)
{
	// SIGALRM 시그널에 대해 handler 등록
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		return -1;
	}

	// 입력받은 값을 다른 함수에도 사용하기 위해 전역변수로 저장
	AlarmSecs = nsecs;

	// nsecs 후에 SIGALRM 시그널을 발생시킴
	alarm(nsecs);

	return 0;
}

main()
{
	printf("Doing something every one seconds\n");

	SetPeriodicAlarm(1);

	for ( ; ; )
		pause();	// 시그널 발생을 기다림
}
