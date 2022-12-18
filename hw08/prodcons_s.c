/* prodcons_s.c
 * Producer & Consumer 예제를 thread와 앞서 구현한 semlib2를 이용해 구현한 프로그램
 * $ gcc –o prodcons_s prodcons_s.c semlib2.c –lpthread (or make prodcons_s)
 * $ ./prodcons_s
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "semlib2.h"
#include "prodcons.h"

BoundedBufferType	Buf;	// Circular Queue -> shared data를 전역변수로 선언
sem_t				EmptySem, FullSem, MutexSem;	// Semaphore

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
void
Producer(void *dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	// critical section에 NLOOPS 만큼 반복 수행
	for (i = 0 ; i < NLOOPS ; i++)  {
		// 버퍼에 빈 공간을 할당받기를 기다림
		if (sem_wait(&EmptySem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		// critical section에 진입하기 위해 mutex semaphore 할당을 기다림
		if (sem_wait(&MutexSem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		// 버퍼에 데이터 쓰기
		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		Buf.buf[Buf.in].data = data;
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;

		// critical section 사용이 끝났으므로 mutex semaphore 반납
		if (sem_post(&MutexSem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
		// 버퍼에 데이터가 채워졌으므로 Full semaphore에 +1 수행
		if (sem_post(&FullSem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

// Bounded Buffer 문제에서 Consumer 역할을 수행할 thread가 수행할 루틴
void
Consumer(void *dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	// critical section에 NLOOPS 만큼 반복 수행
	for (i = 0 ; i < NLOOPS ; i++)  {
		// 버퍼에 데이터가 쓰여지길 기다리기 위해 Full semaphore 할당을 기다림
		if (sem_wait(&FullSem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		// critical section에 진입하기 위해 mutex semaphore 할당을 기다림
		if (sem_wait(&MutexSem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		// 버퍼에서 데이터 꺼내오기
		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;

		// critical section 사용이 끝났으므로 mutex semaphore 반납
		if (sem_post(&MutexSem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
		// 버퍼에 데이터가 비워졌으므로 Empty semaphore에 +1 수행
		if (sem_post(&EmptySem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand()%100)*10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);

	// Empty semaphore 생성
	// 처음엔 모두 비워져있으므로 초기값은 버퍼의 수만큼 지정
	if (sem_init(&EmptySem, 0, MAX_BUF) < 0)  {
		perror("sem_init");
		exit(1);
	}
	// Full semaphore 생성
	// 처음엔 채워져있는 버퍼가 없으므로 초기값을 0으로 지정
	if (sem_init(&FullSem, 0, 0) < 0)  {
		perror("sem_init");
		exit(1);
	}
	// Mutex semaphore 생성. 초기값은 1
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

	// Empty semaphore 제거
	if (sem_destroy(&EmptySem) < 0)  {
		perror("sem_destroy");
	}
	// Full semaphore 제거
	if (sem_destroy(&FullSem) < 0)  {
		perror("sem_destroy");
	}
	// Mutex semaphore 제거
	if (sem_destroy(&MutexSem) < 0)  {
		perror("sem_destroy");
	}
}

