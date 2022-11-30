/* cancel.c
 * pthread_cancel system call을 사용하여 thread를 kill하는 프로그램
 * pthread_setcancelstate, pthread_setcanceltype을 사용하여 thread 생성 시에 cancel 옵션을 설정함
 * $ gcc -o cancel cancel.c -lpthread (or make cancel)
 * $ ./cancel
 */
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

pthread_t	ThreadId[2];

void
SigIntHandler(int signo)
{
	int		i;

	// 어떤 thread가 SIGINT 시그널을 받았는지 출력
	printf("Received a SIGINT signal by thread %d\n", pthread_self());
	printf("Terminate other threads: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

	for (i = 0 ; i < 2 ; i++)  {
		/* Cancel execution of all threads */
		if (pthread_cancel(ThreadId[i]))  {
			perror("pthread_cancel");
			exit(1);
		}
	}
	// 모든 thread가 종료되길 기다림
	for (i = 0 ; i < 2 ; i++)  {
		if (pthread_join(ThreadId[i], NULL))  {
			perror("pthread_join");
			exit(1);
		}
	}
	// 모든 thread 종료 후 메세지 출력
	printf("Threads terminated: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

	exit(0);
}

void
Thread1(void *dummy)
{
	// thread가 생성되면 자신의 thread id를 출력함
	printf("Thread %d created.....\n", pthread_self());

	// thread cancel 가능하도록 설정
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	// cancel시에 thread가 바로 cancel 되도록 설정
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1)
		;
}

void
Thread2(void *dummy)
{
	// thread가 생성되면 자신의 thread id를 출력함
	printf("Thread %d created.....\n", pthread_self());
	
	// thread cancel 가능하도록 설정
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	// cancel시에 thread가 바로 cancel 되도록 설정
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1)
		;
}

main()
{
	// thread1 생성
	if (pthread_create(&ThreadId[0], NULL, (void *)Thread1, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	// thread2 생성
	if (pthread_create(&ThreadId[1], NULL, (void *)Thread2, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	// SIGINT 시그널에 대한 signal handler 등록
	signal(SIGINT, SigIntHandler);

	printf("Press ^C to quit\n");

	for ( ; ; )
		pause();	// 시그널 발생을 기다림
}
