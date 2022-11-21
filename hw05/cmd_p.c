#include <stdio.h>

#define	MAX_CMD		256

/* cmd_p.c
 * child process를 생성하여 command들을 처리하는 command-line 프로그램
 * $ gcc -o cmd_p cmd_p.c (or make cmd_p)
 * $ ./cmd_p
 */

// 입력받은 command를 수행하는(척하는) 함수
void
DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

main()
{
	char	cmd[MAX_CMD];	// 사용자에게 입력받은 command를 저장할 문자열 변수
	int		pid;	// 생성한 child process의 process id를 저장할 변수

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	// 사용자가 command를 입력할 때까지 기다리게 됨
		if (cmd[0] == 'q')	// 사용자가 'q' 입력시 프로그램 종료
			break;

		if ((pid = fork()) < 0)  {	// child process를 생성하고 process id를 pid에 저장
			perror("fork");	// 오류 메세지 출력
			exit(1);
		}
		else if (pid == 0)  {	// child process는 입력받은 command를 수행하고 종료
			DoCmd(cmd);
			exit(0);
		}
#if 0
		else  {	// 위의 if의 값을 1로 바꾸면 아래의 코드가 실행됨
			wait(NULL);	// child process가 종료되길 기다림 -> cmd_i 프로그램과 동일해짐
		}
#endif	// parent process는 child process를 생성한 후에
		// 바로 반복문의 시작으로 돌아가 다른 command의 입력을 기다림
	}
}

