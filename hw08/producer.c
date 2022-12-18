/* producer.c
 * Producer & Consumer 예제를 multi-process 버전으로 구현한 프로그램
 * synchronization을 수행하지 않으므로 critical section problem 발생 가능
 * busy waiting으로 인한 오버헤드 발생 가능
 * $ gcc -o producer producer.c (or make producer)
 * $ gcc -o consumer consumer.c (or make consumer)
 * $ ./consumer
 * $ ./producer
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

main()
{
	BoundedBufferType	*pBuf;	// Circular Queue
	int					shmid, i, data;	// shm id, iterator, buffer data

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

	srand(0x8888);	// 난수 생성
	for (i = 0 ; i < NLOOPS ; i++)  {
		// critical section
		// circular queue에 데이터가 모두 차있는 경우
		if (pBuf->counter == MAX_BUF)  {
			printf("Producer: Buffer full. Waiting.....\n");
			while (pBuf->counter == MAX_BUF)
				;	// busy waiting
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		// circular queue에 데이터 write
		pBuf->buf[pBuf->in].data = data;
		pBuf->in = (pBuf->in + 1) % MAX_BUF;
		pBuf->counter++;

		usleep(data);
		// critical section
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2);
	printf("Producer: %d items in buffer.....\n", pBuf->counter);
	
	// shared memory 반납
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
}
