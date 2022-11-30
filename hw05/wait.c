#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/* wait.c
 * child process가 종료되길 기다렸다 child의 종료 status를 출력하는 프로그램
 * $ gcc -o wait wait.c (or make wait)
 * $ ./wait
 */
main()
{
	pid_t	pid;	// 생성한 child process의 process id를 저장할 변수
	int		status;		// child process의 종료 status를 저장할 integer 변수

	if ((pid = fork()) < 0)  {	// child process를 생성하고 process id를 pid에 저장
		perror("fork");		// 오류 메세지 출력
		exit(1);
	}
	else if (pid == 0)  {	// child process
		printf("I'm a child\n");
		sleep(2);
	}
	else  {		// parent process
		// child process가 끝나길 기다렸다가 종료 status를 status 변수에 저장
		wait(&status);
		// child process의 종료 status를 출력
		printf("A child killed with %d status\n", status);
	}
}
