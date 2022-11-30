#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/* worace.c (without race)
 * process의 synchronous한 특성으로 인한 race condition이 발생하지 않도록
 * wait system call을 사용하여 작성된 프로그램
 * 두 process가 shared resource를 순차적으로 사용하게 됨
 * $ gcc -o worace worace.c (or make worace)
 * $ ./worace
 */

// 한 번에 한 문자씩 출력하고 문자를 출력할 때마다 loop를 수행하는 함수
void
CharAtaTime(char *str)
{
	char	*ptr;
	int		c, i;

	setbuf(stdout, NULL);
	for (ptr = str ; c = *ptr++ ; )  {
		for(i = 0 ; i < 999999 ; i++)
			;
		putc(c, stdout);
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
		wait(NULL);		// child process가 종료되길 기다림
		CharAtaTime("output from parent\n");
	}

	// wait system call을 사용하지 않았다면 synchronization problem가 발생하여
	// 두 process의 출력 결과가 뒤섞였을 것임
	// 해당 프로그램은 wait를 사용함으로써 shared resource를 순차적으로 사용하였으므로
	// race condition이 발생하지 않아 순차적으로 각각의 출력이 정상적으로 보여진다
}
