/* consumer.c
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

	// shared memory 생성
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// shared memory와 circular queue를 매핑
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	srand(0x9999);	// 난수 생성
	for (i = 0 ; i < NLOOPS ; i++)  {
		// critical section
		// circular queue가 모두 비어있는 경우
		if (pBuf->counter == 0)  {
			printf("Consumer: Buffer empty. Waiting.....\n");
			while (pBuf->counter == 0)
				;	// busy waiting
		}

		printf("Consumer: Consuming an item.....\n");
		// circular queue에서 데이터 빼내오기
		data = pBuf->buf[pBuf->out].data;
		pBuf->out = (pBuf->out + 1) % MAX_BUF;
		pBuf->counter--;

		usleep((rand()%100)*10000);
		// critical section
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
}
