/* shm.c
 * shared memory 생성, attach, detach, 삭제하는 예제 프로그램
 * $ gcc -o shm shm.c (or make shm)
 * $ ./shm
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R | SHM_W)		// read, write 권한 지정

char	Array[ARRAY_SIZE];

main()
{
	int		shmid;
	char	*ptr, *shmptr;

	// malloc을 통한 동적할당
	if ((ptr = (char *)malloc(MALLOC_SIZE)) == NULL)  {
		perror("malloc");
		exit(1);
	}

	// shared memory 생성. 다른 프로세스에서 사용 불가.
	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// 생성한 shared memory를 attach
	if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	// Array는 전역변수이므로 Data Segment 공간에 저장됨
	printf("Array[] from %p to %p\n", &Array[0], &Array[ARRAY_SIZE]);
	// shmid는 지역변수이므로 Stack 공간에 저장됨
	printf("Stack around %p\n", &shmid);
	// ptr은 동적할당받은 변수이므로 Heap 공간에 저장됨
	printf("Malloced from %p to %p\n", ptr, ptr+MALLOC_SIZE);
	// shmptr은 shared memory이므로 stack과 heap 공간의 중간 위치에 저장됨
	printf("Shared memory attached from %p to %p\n", shmptr, shmptr+SHM_SIZE);

	// shared memory를 detach
	if (shmdt(shmptr) < 0)  {
		perror("shmdt");
		exit(1);
	}

	// shared memory를 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
}
