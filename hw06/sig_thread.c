/* sig_thread.c
 * Thread를 지정하여 signal handler를 등록하는 프로그램
 * 결과적으로는 어떤 thread를 지정해도 signal handler는 main thread에서 수행됨
 * 리눅스에서는 시그널을 무조건 main thread에서 처리함
 * $ gcc -o sig_thread sig_thread.c -lpthread (or make sig_thread)
 * $ ./sig_thread
 */
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#define	THREAD_MAIN
/*
#define	THREAD_1
#define	THREAD_2
*/


void
SigIntHandler(int signo)
{
	printf("Received a SIGINT signal by thread %d\n", pthread_self());
	printf("Terminate this process\n");

	exit(0);
}

void
Thread1(void *dummy)
{
#ifdef	THREAD_1
	signal(SIGINT, SigIntHandler);	// thread1에 handler 등록
#endif

	while (1)
		;
}

void
Thread2(void *dummy)
{
#ifdef	THREAD_2
	signal(SIGINT, SigIntHandler);	// thread2에 handler 등록
#endif

	while (1)
		;
}

main()
{

	pthread_t	tid1, tid2;

	// thread1 생성
	if (pthread_create(&tid1, NULL, (void *)Thread1, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	// thread2 생성
	if (pthread_create(&tid2, NULL, (void *)Thread2, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	// thread1, thread2의 thread id를 출력
	printf("Create two threads: tid1=%d, tid2=%d\n", tid1, tid2);
	// main thread의 thread id를 출력
	printf("Main thread: tid=%d\n", pthread_self());

#ifdef	THREAD_MAIN
	signal(SIGINT, SigIntHandler);	// main thread에 handler 등록
#endif

	printf("Press ^C to quit\n");

	for ( ; ; )
		pause();	// 시그널 발생을 기다림
}
