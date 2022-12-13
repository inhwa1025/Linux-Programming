/* dining.c
 * 밥먹는 철학자 예시에서 젓가락이 아닌 철학자와 critical section에 대해
 * semaphore를 설정하여 구현한 dining2 프로그램
 * Hold & Wait의 가능성을 제거하였으므로 Deadlock은 발생하지 않음
 * 하지만 starvation 발생의 가능성이 있음
 * $ gcc -o dining2 dining2.c -lpthread (or make dining2)
 * $ ./dining2
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5	// 철학자의 수 -> semaphore의 수
#define	NLOOPS		5	// 반복 횟수

#define	LEFT(i)		((i+NUM_MEN-1)%NUM_MEN)	// i번째 철학자 기준 왼쪽 젓가락
#define	RIGHT(i)	((i+1)%NUM_MEN)	// i번째 철학자 기준 오른쪽 젓가락

enum  { THINKING, EATING, HUNGRY };	// state를 나타내기위한 enum

// 철학자 별로 semaphore 생성. 0으로 초기화. 0: 먹을 수 없음. 1: 먹음
sem_t	Philosopher[NUM_MEN];
sem_t	Mutex;	// critical section 보호를 위한 binary semaphore
int		State[NUM_MEN];	// 철학자의 현재 상태 (enum으로 표현)

// 특정 thread를 usecs 동안 sleep 시키는 함수
void
ThreadUsleep(int usecs)
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

// 랜덤한 시간동안 대기
void
Thinking(int id)
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

// 랜덤한 시간동안 먹는 상태 유지
void
Eating(int id)
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

// id 번째 철학자가 먹을 수 있는 상태인지 test하는 함수
void
Test(int id)
{
	// 내가 배고픈 상태인지 먼저 확인한 후 양쪽 철학자가 먹는 중인지 확인
	// 둘 중 한명이 먹는 상태라면 id번째 철학자는 먹을 수 없음
	// 무한정 기다릴 가능성 존재 -> starvation 발생
	if (State[id] == HUNGRY && State[LEFT(id)] != EATING 
			&& State[RIGHT(id)] != EATING)  {
		// 먹기 가능하므로 상태 변경
		State[id] = EATING;

		// 먹을 수 있으므로 Philosopher semaphore를 1 증가 시킴
		if (sem_post(&Philosopher[id]) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
}

// 왼쪽, 오른쪽 젓가락 들기
void
PickUp(int id)
{
	// critical section : Test 도중 양쪽 철학자의 state가 바뀌면 안됨
	if (sem_wait(&Mutex) < 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	// state 변경
	State[id] = HUNGRY;

	// 먹을 수 있는지 test
	Test(id);

	// 먹을 수 없다면 sem_post가 작동하지 않으므로 대기하게 됨
	if (sem_post(&Mutex) < 0)  {
		perror("sem_post");
		pthread_exit(NULL);
	}

	// 먹고 있는 철학자가 putdown하기를 기다림
	if (sem_wait(&Philosopher[id]) < 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}
}

// 왼쪽, 오른쪽 젓가락 내려놓기
void
PutDown(int id)
{
	// critical section
	if (sem_wait(&Mutex) < 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	// 다 먹었으므로 상태 변경
	State[id] = THINKING;

	// 기다리는 thread를 깨우기위해 test를 수행하여 sem_post가 동작하도록 함
	Test(LEFT(id));
	Test(RIGHT(id));

	// critical section 종료
	if (sem_post(&Mutex) < 0)  {
		perror("sem_post");
		pthread_exit(NULL);
	}
}

void
DiningPhilosopher(int *pId)
{
	int		i;
	int		id = *pId;

	for (i = 0 ; i < NLOOPS ; i++)  {
		// 랜덤한 시간동안 thinking state로 sleep
		Thinking(id);

		// hungry state로 변경 후 먹을 준비를 하는 과정
		PickUp(id);

		// 무작위 시간동안 eating 상태로 sleep
		Eating(id);

		// eating을 끝내고 semaphore 반납 후 양쪽 철학자 thread를 깨운 뒤
		// 다시 thinking state로 변경
		PutDown(id);
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid[NUM_MEN];
	int			i, id[NUM_MEN];

	srand(0x8888);

	// 철학자의 수 만큼 semaphore 생성 후 0으로 초기화
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_init(&Philosopher[i], 0, 0) < 0)  {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}
	// Mutex semphore를 1로 초기화
	if (sem_init(&Mutex, 0, 1) < 0)  {
		perror("sem_init");
		exit(1);
	}

	// DiningPhilosopher를 실행하는 thread를 철학자 수 만큼 생성
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, 
				(void *)&id[i]) < 0)  {
			perror("pthread_create");
			exit(1);
		}
	}

	// 모든 thread의 종료를 기다림
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (pthread_join(tid[i], NULL) < 0)  {
			perror("pthread_join");
			exit(1);
		}
	}

	// 생성한 Philosopher semaphore 제거
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_destroy(&Philosopher[i]) < 0)  {
			perror("sem_destroy");
		}
	}

	// 생성한 Mutex semaphore 제거
	if (sem_destroy(&Mutex) < 0)  {
		perror("sem_destroy");
		exit(1);
	}

}
