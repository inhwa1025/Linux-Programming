/* dining.c
 * 밥먹는 철학자 예시에서 젓가락에 semaphore를 설정해 구현한 dining 프로그램
 * 젓가락마다 한 명만 접근하도록 만들 순 있으나
 * 모든 철학자가 동시에 한 쪽 젓가락만 들고
 * 다른 젓가락을 (서로를) 무한히 기다리는 상태 발생 가능
 * -> deadlock 필요조건이 모두 만족되므로 deadlock 발생 가능성 있음
 * $ gcc -o dining dining.c -lpthread (or make dining)
 * $ ./dining
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5	// 철학자의 수
#define	NLOOPS		5	// 반복할 횟수

sem_t	ChopStick[NUM_MEN];	// 젓가락 -> shared data -> semaphore

// thread를 usecs 동안 sleep하도록 하는 함수
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

void
Thinking(int id)
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

void
Eating(int id)
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

void
DiningPhilosopher(int *pId)
{
	int		i;
	int		id = *pId;

	for (i = 0 ; i < NLOOPS ; i++)  {

		Thinking(id);	// 생각하기

		// 왼쪽 젓가락 들기
		if (sem_wait(&ChopStick[id]) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		// 오른쪽 젓가락 들기
		if (sem_wait(&ChopStick[(id+1) % NUM_MEN]) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		Eating(id);		// 밥먹기

		// 왼쪽 젓가락 내려놓기
		if (sem_post(&ChopStick[id]) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
		// 오른쪽 젓가락 내려놓기
		if (sem_post(&ChopStick[(id+1) % NUM_MEN]) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid[NUM_MEN];
	int			i, id[NUM_MEN];

	srand(0x8888);	// 난수 생성

	// 젓가락에 대한 semaphore를 생성 후 1로 초기화
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_init(&ChopStick[i], 0, 1) < 0)  {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}

	// thread를 철학자 수(5명) 만큼 생성
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

	// semaphore 제거
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_destroy(&ChopStick[i]) < 0)  {
			perror("sem_destroy");
		}
	}
}
