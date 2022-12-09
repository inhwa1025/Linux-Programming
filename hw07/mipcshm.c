/* mipcshm.c
 * shared memory에 parent process와 child process가
 * 번갈아가며 변수를 1씩 증가시키는 기존의 mipc.c 프로그램을
 * memory-mapped file 대신 shmget, shmat을 사용하여 구현한 방법
 * $ gcc -o mipcshm mipcshm.c synclib.c
 * $ ./mipcshm
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "synclib.h"
#include "shm.h"

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
	int		shmid, i, counter;
	pid_t	pid;
	char	*area;

	// shared memory 생성. 다른 프로세스에서 사용 불가.
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}
	// 생성한 shared memory를 attach
	if((area = shmat(shmid, NULL, 0)) == (void*)-1) {
		perror("shmget");
		exit(1);
	}

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

	// shared memory를 detach
	if (shmdt(area) < 0)  {
		perror("shmdt");
		exit(1);
	}

	// shared memory를 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
}
