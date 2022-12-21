/* sipc_server_sem.c
 * hw07의 sipc1 예제를 semaphore 사용을 통해 
 * flag와 busy waiting를 제거하여 구현한 프로그램.
 * 두 개의 process (server, client)가 shared memory로 통신.
 * 해당 프로그램은 server에 해당하며 reply를 보내고,
 * client 프로그램으로부터 request를 받아 출력함.
 * $ gcc -o sipc_server_sem sipc_server_sem.c semlib.c
 * $ ./sipc_server_sem
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sipc.h"
#include "semlib.h"

int main()
{
	// 통신을 위해 메세지를 주고받을 shared memory의 id
	int		shmid;
	// shared memory의 virtual address 매핑과 데이터 입력을 위함
	char	*ptr;
	// client가 request를 보낸 후 +1 하면 server가 wait을 멈추고
	// request를 처리하도록 하기 위한 semaphore. 
	// 처음에는 도착한 request가 없으므로 초기값은 0
	int requestSemid;
	// server가 reply를 보낸 후 +1 하면 client가 wait를 멈추고 
	// reply를 받을 수 있도록 하기 위한 semaphore.
	// 처음에는 도착한 reply가 없으므로 초기값은 0
	int replySemid;
	// shared memory(critical section)을 보호를 위한 semaphore. 초기값 1
	int mutexSemid;

	// client에서 생성한 shared memory id 받아오기
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}
	// shared memory를 ptr에 mapping
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1) {
		perror("shmat");
		exit(1);
	}
	// client에서 생성한 semaphore id 받아오기
	if ((requestSemid = semInit(REQUEST_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((replySemid = semInit(REPLY_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	// client로부터 request를 받을 때까지 wait -> repuest semaphore 할당받기
	if (semWait(requestSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	// 받은 request에 접근하기 위해 shared memory에 접근해야하므로
	// critical section 접근을 위한 mutex semaphore 할당받기
	if (semWait(mutexSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	// 받은 request 메세지 출력하고 reply 보내기
	printf("Server: Received request: %s.....\n", ptr);
	sprintf(ptr, "This is a reply from %d/.", getpid());

	// critical section 사용이 끝났으므로 mutex semaphore 반납
	if (semPost(mutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	// client에 reply 전송을 알리기 위해 reply semaphore에 +1 수행
	if (semPost(replySemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}
	printf("Server: Replied.\nProgram ends in 2 seconds...\n");

	// client 종료를 기다리기 위해 2초동안 sleep
	sleep(2);

	// 모든 semaphore 제거
	if (semDestroy(requestSemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
		exit(1);
	}
	if (semDestroy(replySemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
		exit(1);
	}
	if (semDestroy(mutexSemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
		exit(1);
	}
	// shared memory 제거
	if(shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
