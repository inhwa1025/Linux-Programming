/* sipc1.c
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

	// shared memory 생성
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

	pInt = (int *)ptr;	// shared memory의 flag
	// flag의 값이 1으로 바뀔때까지 무한루프를 돌며 기다림
	while ((*pInt) == 0)
		;
	// -> low level lock (spin lock. busy waiting) -> CPU가 낭비됨
	// -> OS에서 제공하는 semaphore를 사용한 동기화로 변경하는 것이 좋다

	pData = ptr + sizeof(int);	// 다른 프로세스가 shared memory에 write한 데이터 읽어오기
	printf("Received request: %s.....", pData);	// 읽어온 데이터 출력
	// 다른 프로세스에게 보낼 reply를 shared memory에 write
	sprintf(pData, "This is a reply from %d.", getpid());
	*pInt = 0;	// flag를 0으로 변경
	printf("Replied.\n");

	// 다른 프로세스가 shared memory를 사용하는 것을 기다림
	sleep(1);

	/* Detach shared memory */
	if (shmdt(ptr) < 0)  {
		perror("shmdt");
		exit(1);
	}

	/* Remove shared memory */
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
}
