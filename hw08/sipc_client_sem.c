/* sipc_client_sem.c
 * hw07의 sipc2 예제를 semaphore 사용을 통해 
 * flag와 busy waiting를 제거하여 구현한 프로그램.
 * 두 개의 process (server, client)가 shared memory로 통신.
 * 해당 프로그램은 client로 request를 보내고,
 * server 프로그램으로부터 reply를 받아 출력함.
 * $ gcc -o sipc_client_sem sipc_client_sem.c semlib.c
 * $ ./sipc_client_sem
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

	// shared memory 생성
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}
	// shared memory를 ptr에 mapping
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1) {
		perror("shmat");
		exit(1);
	}

	// request, reply, mutex semaphore 생성
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

	// request, reply, mutex semaphore의 값을 초기화
	// request와 reply의 초기값은 0, mutex의 초기값은 1
	if (semInitValue(requestSemid, 0) < 0) {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(replySemid, 0) < 0) {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(mutexSemid, 1) < 0) {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	// critical section 접근을 위해 mutex semaphore 할당받기
	if (semWait(mutexSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	// server에 보낼 request 메세지 생성
	sprintf(ptr, "This is a request from %d.", getpid());
	//critical section 사용이 끝났으므로 mutex semaphore 반납
	if (semPost(mutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	// server에 request 전송을 알리기 위해 reqeust semphore에 +1 수행
	if (semPost(requestSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}
	printf("Client: Sent a request.....\n");

	// server로부터 reply를 받을 때까지 wait -> reply semaphore 할당받기
	if (semWait(replySemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	// 받은 reply에 접근하기 위해 shared memory에 접근해야하므로
	// critical section 접근을 위한 mutex semaphore 할당받기
	if (semWait(mutexSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	// 받은 reply 메세지 출력
	printf("Client: Received reply %s\n", ptr);
	// critical section 사용이 끝났으므로 mutex semaphore 반납
	if (semPost(mutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	// 생성한 semaphore의 제거는 server 프로그램에서 수행됨
}
