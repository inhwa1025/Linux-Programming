// gcc -o app app.c semlib.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "shm.h"

int Calc(int number)
{
	BoundedBufferType   *pBuf;
	int                 shmid, i, data;
	int                 semaphore1, semaphore2;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *)-1) {
		perror("shmat");
		exit(1);
	}

	pBuf->buf[pBuf->in].data = number;
	pBuf->in = (pBuf->in + 1) % MAX_BUF;
	pBuf->counter++;

	if ((semaphore1 = semInit(ONE_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if (semPost(semaphore1) < 0)  {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
	
	if ((semaphore2 = semInit(TWO_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if (semWait(semaphore2) < 0)  {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	data = pBuf->buf[pBuf->out].data;
	pBuf->out = (pBuf->out + 1) % MAX_BUF;
	pBuf->counter--;

	return data;
}

int main(int argc, char *argv[])
{
	int		num, result;

	num = atoi(argv[1]);
	result = Calc(num);
	
	printf("Calc = %d\n", result);
}
