#include <stdio.h>
#include <pthread.h>

/* thread.c
 * thread를 생성하고, thread의 종료를 기다리고, thread를 종료하는 프로그램
 * 서로 다른 thread들에서 consol에 메세지를 출력
 * $ gcc -o thread thread.c -lpthread (or make thread)
 * $ ./thread
 */

// thread에서 수행할 함수를 선언
void
PrintMsg(char *msg)
{
	printf("%s", msg);	// parameter로 전달받은 문자열을 출력

	pthread_exit(NULL);	// thread 종료
}

main()
{
	pthread_t	tid1, tid2;	// 생성한 thread의 id를 저장할 변수
	char		*msg1 = "Hello, ";	// 첫 번째 thread에서 출력할 문자열
	char		*msg2 = "World!\n";	// 두 번째 thread에서 출력할 문자열

	/* Thread ID: tid1, Thread function: PrintMsg, Thread argument: msg1 */
	if (pthread_create(&tid1, NULL, (void *)PrintMsg, (void *)msg1) < 0)  {
		perror("pthread_create");	// 오류 메세지 출력
		exit(1);
	}

	/* Thread ID: tid2, Thread function: PrintMsg, Thread argument: msg2 */
	if (pthread_create(&tid2, NULL, (void *)PrintMsg, (void *)msg2) < 0)  {
		perror("pthread_create");	// 오류 메세지 출력
		exit(1);
	}

	// thread가 생성됐음을 알리고 생성된 thread id 출력
	printf("Threads created: tid=%d, %d\n", tid1, tid2);
	
	/* Wait for tid1 to exit */
	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");	// 오류 메세지 출력
		exit(1);
	}
	/* Wait for tid2 to exit */
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");	// 오류 메세지 출력
		exit(1);
	}
	
	// thread가 종료됐음을 알리고 종료된 thread id 출력
	printf("Threads terminated: tid=%d, %d\n", tid1, tid2);
}

