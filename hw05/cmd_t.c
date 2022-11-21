#include <stdio.h>
#include <pthread.h>

#define	MAX_CMD		256

/* cmd_t.c
 * thread를 생성하여 command들을 처리하는 command-line 프로그램
 * $ gcc -o cmd_t cmd_t.c -lpthread (or make cmd_t)
 * $ ./cmd_t
 */

// 입력받은 command를 수행하는(척하는) 함수
void
DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");

	pthread_exit(NULL);	// thread 종료
}

main()
{
	char		cmd[MAX_CMD];	// 사용자에게 입력받은 command를 저장할 문자열 변수
	pthread_t	tid;	// 생성한 thread의 id를 저장할 변수

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	// 사용자가 command를 입력할 때까지 기다림
		if (cmd[0] == 'q')	// 사용자가 'q' 입력시 프로그램 종료
			break;

		/* Thread ID: tid, Thread function: DoCmd, Thread argument: cmd */
		if (pthread_create(&tid, NULL, (void *)DoCmd, (void *)cmd) < 0)  {
			perror("pthread_create");	// 오류 메세지 출력
			exit(1);
		}

#if 0	// 값을 1로 바꾸면 아래의 코드가 실행됨
		pthread_join(tid, NULL);	// thread가 종료되길 기다림 -> cmd_i 프로그램과 동일해짐
#endif	// process는 thread를 생성한 후에 바로 반복문의 시작으로 돌아가 다른 command의 입력을 기다림
	}
}

