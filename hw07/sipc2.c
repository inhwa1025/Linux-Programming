/* sipc2.c
 * 두 개의 process가 shared memory를 사용하여 통신하는 예제 프로그램
 * $ gcc -o sipc1 sipc1.c (or make sipc1)
 * $ gcc -o sipc2 sipc2.c (or make sipc2)
 * $ ./sipc1
 * $ ./sipc2
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"


main()
{
	int		shmid;
	char	*ptr, *pData;
	int		*pInt;

	// shared memory 생성 또는 존재하는 shared memory를 받아오기
	/* SHM_KEY, SHM_SIZE, SHM_MODE in shm.h */
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// 생성한 shared memory를 attach
	if ((ptr = shmat(shmid, 0, 0)) == (void*)-1)  {
		perror("shmat");
		exit(1);
	}

	pInt = (int *)ptr;	// shared memory의 flag 공간
	pData = ptr + sizeof(int);	// shared memory의data 공간
	// 다른 프로세스에게 보낼 request를 shared memory에 write
	sprintf(pData, "This is a request from %d.", getpid());
	*pInt = 1;	// flag를 1으로 변경
	printf("Sent a request.....");

	// flag의 값이 0으로 바뀔때까지 무한루프를 돌며 기다림
	while ((*pInt) == 1)
		;

	// // 다른 프로세스가 shared memory에 write한 데이터를 읽어와 출력
	printf("Received reply: %s\n", pData);

	/* Detach shared memory */
	if (shmdt(ptr) < 0)  {
		perror("shmdt");
		exit(1);
	}
}
