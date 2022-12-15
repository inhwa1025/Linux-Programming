/* prodcons.c
 * Producer & Consumer 예제를 multi-thread 버전으로 구현한 프로그램
 * POSIX semaphore를 사용하여 synchronization을 수행함
 * $ gcc -o prodcons prodcons.c -lpthread (or make prodcons)
 * $ ./prodcons
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType	Buf;	// Circular Queue를 전역변수로 선언
// EmptySem: empty semaphore. queue에 비어있는 데이터의 수. counting semaphore
// FullSem: full semaphore. queue에 채워져있는 데이터의 수. counting semaphore
// MutexSem: mutex semaphore. critical section 보호를 위한 binary semaphore
sem_t				EmptySem, FullSem, MutexSem;

// atomic하게 usecs mircro second동안 thread를 sleep 시키는 함수
void
ThreadUsleep(int usecs)
{
	// 조건 변수는 상태 정보만을 알리기 위해 사용됨
	// 상태 정보를 atomic하게 주고받기 위해 mutex와 함께 사용
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	// thread가 사용할 조건 변수 생성 및 초기화. mutex와 항상 함께 사용.
	// critical section 안에서 어떤 조건을 만족하는 상황을 만들 때 사용.
	// 다른 thread가 critical section 안에 들어올 수 있도록 만들어줌
	if (pthread_cond_init(&cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	// mutex 변수 생성 및 초기화. critical section 보호를 위해 사용.
	if (pthread_mutex_init(&mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	// 현재 시간 가져오기. pthread_cond_timedwait 함수가 절대 시간을 사용하므로
	// gettimeofday 함수를 사용하여 현재 시간을 가져와야 함.
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	// mutex lock을 확인하고 critical section이 사용가능해질때까지 wait하다가 진입.
	// 다른 thread의 cond_signal에 의해 진입할 수도 있음.
	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	// 절대시간인 ts 동안 thread를 sleep시킴.
	// 도중에 조건(cond)을 만족할 경우 주어진 시간보다 먼저 깨어날 수 있음.
	// 해당 코드에서는 조건을 만족시키지 못하도록해 정해진 시간동안 thread를 sleep
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	// 조건 변수 제거
	if (pthread_cond_destroy(&cond) < 0)  {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	// mutex 변수 제거
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

// Bounded Buffer 문제에서 Producer 역할을 수행할 thread가 수행할 루틴
// semaphore 값을 확인하고 Bounded Buffer에 데이터를 삽입
// semaphore를 사용하므로 busy waiting 없이 수행될 수 있음
void
Producer(void *dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	// ciritical section에 NLOOPS 만큼 반복 수행
	for (i = 0 ; i < NLOOPS ; i++)  {
		// queue에 빈 버퍼 공간이 있는지 확인 (EmptySem의 값이 1 이상인지)
		// 1 이상이라면 EmptySem에 -1 수행. 0이라면 wait
		if (sem_wait(&EmptySem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		// critical section이 사용중인지 확인
		// 1이라면 MutexSem에 -1을 수행하고 critical section에 진입
		// 0이라면 wait
		if (sem_wait(&MutexSem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		// 버퍼에 넣을 데이터 생성
		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		// 버퍼에 데이터 넣기
		Buf.buf[Buf.in].data = data;
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;

		// critical section 사용이 끝났으므로 MutexSem에 +1 수행
		if (sem_post(&MutexSem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
		// 버퍼에 데이터를 write하였으므로 FullSem에 +1 수행
		if (sem_post(&FullSem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);	// thread sleep 시키기
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);
	
	pthread_exit(NULL);
}

// Bounded Buffer 문제에서 Consumer 역할을 수행할 thread가 수행할 루틴
// semaphore 값을 확인하고 Bounded Buffer에 데이터를 삽입
// semaphore를 사용하므로 busy waiting 없이 수행될 수 있음
void
Consumer(void *dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	// ciritical section에 NLOOPS 만큼 반복 수행
	for (i = 0 ; i < NLOOPS ; i++)  {
		// queue에 채워진 버퍼 공간이 있는지 확인 (FullSem의 값이 1 이상인지)
		// 1 이상이라면 FullSem에 -1 수행. 0이라면 wait
		if (sem_wait(&FullSem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		// critical section이 사용중인지 확인
        // 1이라면 MutexSem에 -1을 수행하고 critical section에 진입
        // 0이라면 wait
		if (sem_wait(&MutexSem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		// Bounded Buffer에서 데이터를 읽어옴
		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;

		// critical section 사용이 끝났으므로 MutexSem에 +1 수행
		if (sem_post(&MutexSem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
		// 버퍼에서 데이터를 빼냈으므로 EmptySem에 +1 수행
		if (sem_post(&EmptySem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand()%100)*10000);	// thread sleep 시키기
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x9999);	// 난수 생성

	// Empty semaphore 생성 및 초기화
	// 초기에는 모두 비워져있으므로 MAX_BUF 값으로 초기화
	// 두 번째 파라미터가 0이면 한 프로세스 내의 thread끼리 공유
	// 0이 아닌 경우 프로세스 간에 공유되는 semaphore
	if (sem_init(&EmptySem, 0, MAX_BUF) < 0)  {
		perror("sem_init");
		exit(1);
	}
	// Full semaphore 생성 및 초기화
	// 초기에는 채워져있는 버퍼가 아무것도 없으므로 0으로 초기화
	if (sem_init(&FullSem, 0, 0) < 0)  {
		perror("sem_init");
		exit(1);
	}
	// Mutex semaphore 생성 및 초기화
	// critical section은 1개이므로 1으로 초기화
	if (sem_init(&MutexSem, 0, 1) < 0)  {
		perror("sem_init");
		exit(1);
	}

	// Producer thread 생성
	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	// Consumer thread 생성
	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	// Producer thread의 종료를 기다림
	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	// Consumer thread의 종료를 기다림
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	
	// 모든 thread 종료 후 counter 값을 출력
	printf("Main    : %d items in buffer.....\n", Buf.counter);

	// 모든 thread가 종료되었으므로 생성했던 모든 semaphore 제거
	if (sem_destroy(&EmptySem) < 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&FullSem) < 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) < 0)  {
		perror("sem_destroy");
	}
}
