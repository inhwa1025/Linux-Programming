/* producer_s.c
 * Producer & Consumer 예제를 multi-process 버전으로 구현한 프로그램
 * 앞서 구현한 semlib library를 사용하여 semaphore를 구현
 * semaphore를 사용하여 synchronization을 수행함
 * consumer_s 프로그램이 먼저 실행된 후 producer_s 프로그램 실행 가정
 * $ gcc -o producer_s producer_s.c semlib.c (or make producer_s)
 * $ gcc -o consumer_s consumer_s.c semlib.c (or make consumer_s)
 * $ ./consumer_s
 * $ ./producer_s
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

main()
{
	BoundedBufferType	*pBuf;	// Circular Queue
	int					shmid, i, data;	// shared memory
	// emptySemid: empty semaphore. queue에 비어있는 데이터의 수. counting semaphore
	// fullSemid: full semaphore. queue에 채워져있는 데이터의 수. counting semaphore
	// mutexSemid: mutex semaphore. critical section 보호를 위한 binary semaphore
	int					emptySemid, fullSemid, mutexSemid;

	// shared memory id 받아오기
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// shared memory와 circular queue를 매핑
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	// semaphore id 받아오기
	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	srand(0x8888);	// 난수생성
	// ciritical section에 NLOOPS 만큼 반복 수행
	for (i = 0 ; i < NLOOPS ; i++)  {
		// queue에 빈 버퍼 공간이 있는지 확인 (emptySemid의 값이 1 이상인지)
		// 1 이상이라면 emptySemid에 -1 수행. 0이라면 wait
		if (semWait(emptySemid) < 0)  {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		// critical section이 사용중인지 확인
		// 1이라면 mutexSemid에 -1을 수행하고 critical section에 진입
		// 0이라면 wait
		if (semWait(mutexSemid) < 0)  {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}

		// 버퍼에 넣을 데이터 생성
		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		// 버퍼에 데이터 넣기
		pBuf->buf[pBuf->in].data = data;
		pBuf->in = (pBuf->in + 1) % MAX_BUF;
		pBuf->counter++;

		// critical section 사용이 끝났으므로 mutexSemid에 +1 수행
		if (semPost(mutexSemid) < 0)  {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		// 버퍼에 데이터를 write하였으므로 fullSemid에 +1 수행
		if (semPost(fullSemid) < 0)  {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		usleep(data);
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2);
	printf("Producer: %d items in buffer.....\n", pBuf->counter);
	
	// 생성했던 모든 semaphore 제거
	if (semDestroy(emptySemid) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(fullSemid) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(mutexSemid) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	// shared memory 제거
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
}
