#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/* fork1.c
 * child process를 생성하고 process id를 출력하는 프로그램
 */
main()
{
	int		pid;	// 생성한 child process의 process id를 저장할 변수

	if ((pid = fork()) < 0)  {	// child process를 생성하고 process id를 pid에 저장
		perror("fork");				// 오류 메세지 출력
		exit(1);
	}
	else if (pid == 0)  {			// child process
		/* child */
		// 자신의 process id와 parent process의 process id를 출력
		printf("I am %d. My parent is %d.\n", getpid(), getppid());
	}
	else  {							// parent process
		/* parent */
		// 자신의 process id와 child process의 process id를 출력
		printf("I am %d. My child is %d.\n", getpid(), pid);
	}
}
