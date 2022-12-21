/* sipc_mutex_t.c
 * hw07의 sipc1, sipc2 예제를 multi-thread 버전으로 구현한 프로그램
 * shared memory 대신 전역변수를 사용
 * mutex & condition variable을 이용하여 synchronization 수행
 * -----------------------------------------------------------------
 * mutex & condition variable을 사용하여 구현한 프로그램과 비교
 *  - semaphore 버전보다 mutex & cond 버전이 더 직관적이다.
 *  - semaphore 버전은 mutex 할당 이전에 다른 semaphore들에 대해
 *    먼저 할당을 수행해야 한다.
 *  - 코드를 읽을 때 critical section보다 추가 조건을 먼저 보게된다.
 *  - critical section을 읽고 다시 조건을 읽고 반복적으로 읽게 된다.
 *  - mutex & cond 버전은 mutex lock을 먼저 하고 이후 조건변수 처리.
 *  - semaphore 버전보다 mutex & cond 버전이 가독성이 더 높을 것.
 *  - mutex&cond 버전은 변수 type, 이름과 함수 이름이 분리되어 있어
 *    mutex 변수와 조건 변수의 구분이 쉽다.
 *  - 반면 semaphore 버전은 모두 semaphore type을 사용하고 같은 함수
 *    를 사용하므로 코드를 읽을 때 헷갈릴 수 있다.
 *  - semaphore는 초기화가 필요하지만 mutex&cond는 초기화가 필요없음
 *    semaphore는 개발자가 용도와 의미를 부여하므로 읽기 어렵고
 *    mutex는 용도와 의미가 고정되어있으므로 읽기 쉽다.
 *  - semaphore의 사용 폭이 더 크다. mutex는 binary semaphore와 매우
 *    유사하며, counting semaphore는 mutex로 나타낼 수 없다.
 * -----------------------------------------------------------------
 * $ gcc -o sipc_mutex_t sipc_mutex_t.c -lpthread
 * $ ./sipc_mutex_t
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_BUF 1024

// thread간 메세지를 주고받기 위한 shared data를 전역변수로 선언
char				Buf[MAX_BUF];
// client가 request를 보낸 것을 server에게 알리기 위한 조건변수
pthread_cond_t		SendRequest;
// server가 reply를 보낸 것을 client에게 알리기 위한 조건변수
pthread_cond_t		SendReply;
// shared data(critical section) 보호를 위한 mutex
pthread_mutex_t		Mutex;

// server 역할을 수행할 thread가 수행할 루틴
void
Server(void *dummy)
{
	printf("Server: Start..... tid: %ld\n", pthread_self());

	// critical section 접근을 위해 mutex lock
	if (pthread_mutex_lock(&Mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	// Request가 없는 경우 mutex lock을 해제하여 
	// 다른 thread가 critical section에 접근할 수 있도록 함
	// Request를 받을때까지 기다림
	if (pthread_cond_wait(&SendRequest, &Mutex) < 0) {
		perror("pthread_cond_wait");
		pthread_exit(NULL);
	}

	// 받은 request 메세지 출력
	printf("Server: Received request: %s.....\n", Buf);

	// client에게 보낼 reply 작성
	sprintf(Buf, "This is a reply from %ld.", pthread_self());

	// reply를 작성하였으므로 SendReply 시그널을 보냄
	if (pthread_cond_signal(&SendReply) < 0) {
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}

	// critical section을 빠져나왔으므로 mutex unlock
	if (pthread_mutex_unlock(&Mutex) < 0) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}

	printf("Server: Reply done. server down.....\n");

	pthread_exit(NULL);
}

// client 역할을 수행할 thread가 수행할 루틴
void
Client(void *dummy)
{
	printf("Client: Start..... tid: %ld\n",pthread_self());

	// critical section 접근을 위해 mutex lock
	if (pthread_mutex_lock(&Mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	// server에 보낼 request 메세지 작성
	printf("Client: Sending a request.....\n");
	sprintf(Buf, "This is a request from %ld.", pthread_self());

	// request를 보냈으므로 SendRequest 시그널을 보냄
	if (pthread_cond_signal(&SendRequest) < 0) {
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}

	// critical section을 빠져나왔으므로 mutex unlock
	if (pthread_mutex_unlock(&Mutex) < 0) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}

	// critical section 접근을 위해 mutex lock
	if (pthread_mutex_lock(&Mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	// Reply를 받을때까지 기다림
	if (pthread_cond_wait(&SendReply, &Mutex) < 0) {
		perror("pthread_cond_wait");
		pthread_exit(NULL);
	}

	printf("Client: received reply. Reply: %s\n", Buf);

	// critical section을 빠져나왔으므로 mutex unlock
	if (pthread_mutex_unlock(&Mutex) < 0) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}

	printf("Client: client down.....\n");

	pthread_exit(NULL);
}

int
main()
{
	pthread_t	tid1, tid2;

	// SendRequest 조건변수 생성
	if (pthread_cond_init(&SendRequest, NULL) < 0) {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	// SendReply 조건변수 생성
	if (pthread_cond_init(&SendReply, NULL) < 0) {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	// Mutex 생성
	if (pthread_mutex_init(&Mutex, NULL) < 0) {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	// Server thread 생성
	if (pthread_create(&tid1, NULL, (void *)Server, (void *)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	// Client thread 생성
	if (pthread_create(&tid2, NULL, (void *)Client, (void *)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	// Server thread의 종료를 기다림
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	// Client thread의 종료를 기다림
	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	printf("Main : All Threads Exit...\n");

	// SendRequest 조건변수 제거
	if (pthread_cond_destroy(&SendRequest) < 0) {
		perror("pthread_cond_destroy");
	}
	// SendReply 조건변수 제거
	if (pthread_cond_destroy(&SendReply) < 0) {
		perror("pthread_cond_destroy");
	}
	// Mutex 제거
	if (pthread_mutex_destroy(&Mutex) < 0) {
		perror("pthread_mutex_destroy");
	}
}
