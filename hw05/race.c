#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/* race.c
 * process의 synchronous한 특성을 이용해 race condition을 발생시키는 프로그램
 * $ gcc -o race race.c (or make race)
 * $ ./race
 */

// 한 번에 한 문자씩 출력하고 문자를 출력할 때마다 loop를 수행하는 함수
void
CharAtaTime(char *str)
{
	char	*ptr;
	int		c, i;

	setbuf(stdout, NULL);
	for (ptr = str ; c = *ptr++ ; )  {
		// loop를 수행하는 동안 할당받은 time quantum을 모두 사용하게 됨
		// -> 다른 프로세스가 수행되도록 차례가 넘어감
		// => Scheduling에 의한 context switching
		for(i = 0 ; i < 999999 ; i++)
			;
		putc(c, stdout);	// 문자 하나를 출력
	}
}

main()
{
	pid_t	pid;	// 생성한 child process의 process id를 저장할 변수

	if ((pid = fork()) < 0)  {	// child process를 생성하고 process id를 pid에 저장
		perror("fork");		// 오류 메세지 출력
		exit(1);
	}
	else if (pid == 0)  {	// child process
		CharAtaTime("output from child\n");
	} 
	else  {		// parent process
		CharAtaTime("output from parent\n");
	}

	// 두 process간 shared resource는 console. parent와 child가 서로 console에 메세지를 보냄
	// child process의 출력 결과와 parent process의 출력결과가 섞임
	// race condition으로 인한 synchronization problem 발생
	// 순차적으로 한 번에 하나의 process만 사용하도록 해야함
}

