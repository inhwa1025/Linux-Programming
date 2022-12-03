/* mipc.c
 * shared memory에 parent process와 child process가
 * 번갈아가며 변수를 1씩 증가시키는 프로그램
 * shmget, shmat 대신 memory-mapped file을 사용하여 구현한 방법
 * $ gcc -o mipc mipc.c synclib.c (or make mipc)
 * $ ./mipc
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "synclib.h"

#define	NLOOPS	10
#define	SIZE	sizeof(long)

// shared memory의 값을 1 증가시키는 update 함수
int
update(long *ptr)
{
	return ((*ptr)++);
}

main()
{
	int		fd, i, counter;
	pid_t	pid;
	caddr_t	area;

	// /dev/zero file은 mmap시 shared memory가 되는 UNIX의 특수 파일
	if ((fd = open("/dev/zero", O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}
	// /dev/zero file을 memory에 mapping. read/write 가능.
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	close(fd);	// file close

	TELL_WAIT();
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid > 0)  {	// parent
		for (i = 0 ; i < NLOOPS ; i += 2)  {
			if ((counter = update((long *) area)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD();
			WAIT_CHILD();
		}
	}
	else  {		// child
		for (i = 1 ; i < NLOOPS ; i += 2)  {
			WAIT_PARENT();
			if ((counter = update((long *) area)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT();
		}
	}
}
