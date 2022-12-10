/* sync.c
 * parent process와 child process가 5번 번갈아가면서 수행되도록
 * synclib.c에 synchronization library를 pipe로 구현한 프로그램
 * $ gcc -o sync sync.c synclib.c (or make sync)
 * $ ./sync
 */
#include <stdio.h>
#include "synclib.h"

#define	NLOOPS	5	// 번갈아가면서 수행되는 것이 5번 반복됨


main()
{
	int		i;		// iterator
	pid_t	pid;	// process id

	TELL_WAIT();	// synclib.c에 구현된 pipe 생성 함수
	// -> pipe가 2개 생성 됨 (pipe1, pipe2)
	if ((pid = fork()) < 0)  {	// child process 생성
		perror("fork");
		exit(1);
	}
	else if (pid > 0)  {	// parent process
		for (i = 0 ; i < NLOOPS ; i++)  {
			TELL_CHILD();	// synclib.c에 구현된 pipe1에 데이터를 write하는 함수
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD();	// synclib.c에 구현된 pipe2에서 데이터를 read하는 함수
		}
	}
	else  {		// child process
		for (i = 0 ; i < NLOOPS ; i++)  {
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT();	// synclib.c에 구현된 pipe1에서 데이터를 read하는 함수
			TELL_PARENT();	// synclib.c에 구현된 pipe2에 데이터를 write하는 함수
			printf("Child: Tell to parent\n");
		}
	}
}
