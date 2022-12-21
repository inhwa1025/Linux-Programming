/* myusleep.c
 * select system call을 사용해 usleep을 구현한 프로그램
 * $ gcc -o myusleep myusleep.c (or make myusleep)
 * $ ./myusleep
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

// 입력받은 시간만큼 sleep하는 함수
void
myusleep(int secs, int usecs)
{
	struct timeval	tm;	// 상대시간

	tm.tv_sec = secs;
	tm.tv_usec = usecs;

	// select를 sleep처럼 사용. 발생할 수 없는 socket을 tm 시간동안 기다림
	if (select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tm) < 0)  {
		perror("select");
		return;
	}
}

main()
{
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000);
}
