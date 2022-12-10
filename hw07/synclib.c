/* synclib.c
 * synchronization library를 pipe로 구현한 프로그램
 * $ gcc -o sync sync.c synclib.c (or make sync)
 * $ ./sync
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

static int	Pfd1[2], Pfd2[2];	// 생성한 pipe1, pipe2의 주소가 저장될 배열

// pipe 생성 함수 -> pipe가 2개 생성 됨 (pipe1, pipe2)
// pipe1: parent가 write하면 child가 read. read - Pfd1[0], write - Pfd1[1]
// pipe2: child가 wirte하면 parent가 read. read - Pfd2[0], write - Pfd2[1]
void
TELL_WAIT(void)
{
	// 2개의 pipe 중 어느 하나라도 정상적으로 생성되지 않으면 프로그램 종료
	if (pipe(Pfd1) < 0 || pipe(Pfd2) < 0)  {
		perror("pipe");
		exit(1);
	}
}

// pipe2에 데이터를 write하는 함수
void
TELL_PARENT(void)
{
	if (write(Pfd2[1], "c", 1) != 1)  {
		perror("write");
		exit(1);
	}
}

// pipe1에서 데이터를 read하는 함수
void
WAIT_PARENT(void)
{
	char	c;

	if (read(Pfd1[0], &c, 1) != 1)  {
		perror("read");
		exit(1);
	}
	if (c != 'p')  {
		fprintf(stderr, "WAIT_PARENT: incorrect data");
		exit(1);
	}
}

// pipe1에 데이터를 write하는 함수
void
TELL_CHILD(void)
{
	if (write(Pfd1[1], "p", 1) != 1)  {
		perror("write");
		exit(1);
	}
}

// pipe2에서 데이터를 read하는 함수
void
WAIT_CHILD(void)
{
	char	c;

	if (read(Pfd2[0], &c, 1) != 1)  {
		perror("read");
		exit(1);
	}
	if (c != 'c')  {
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}
