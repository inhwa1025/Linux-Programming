// gcc -o rpc rpc.c semlib.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "semlib.h"
#include "shm.h"

int		shmid, i, data;
int		semaphore1, semaphore2;

void
CloseServer()
{
	if (semDestroy(semaphore1) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(semaphore2) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}

	exit(0);
}

int main()
{
	BoundedBufferType	*pBuf;
	
	signal(SIGINT, CloseServer);

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	if ((semaphore1 = semInit(ONE_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((semaphore2 = semInit(TWO_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	
	while (1) {
		if (semWait(semaphore1) < 0)  {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		
		int num = 0;
		num = pBuf->buf[pBuf->out].data;
		pBuf->out = (pBuf->out + 1) % MAX_BUF;
		pBuf->counter--;

		// 합 계산
		int sum = 0;
		for (int i=0; i < (num+1); i++) {
			sum += i;
		}
		
		pBuf->buf[pBuf->in].data = sum;
		pBuf->in = (pBuf->in + 1) % MAX_BUF;
		pBuf->counter++;

		printf("Calc = %d\n", sum);
		
		if (semPost(semaphore2) < 0)  {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
	}
}
