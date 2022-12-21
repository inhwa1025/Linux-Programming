// mutex와 condition variable을 사용하여 POSIX semaphore를 구현한 것
// $ gcc -c semlib.c (or make semlib2.o)
#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"


// semaphore를 생성하는 함수
int
sem_init(sem_t *sem, int pshared, int value)
{
	if (pshared)  {
		fprintf(stderr, "Function not supported\n");
		return -1;
	}

	// semaphore counter는 critical section이므로 보호를 위한 mutex 생성
	if (pthread_mutex_init(&sem->mutex, NULL) < 0)
		return -1;

	// condition variable 생성
	if (pthread_cond_init(&sem->cond, NULL) < 0)
		return -1;

	// semaphore counter 값 초기화
	sem->sval = value;

	return 0;
}

// semaphore를 할당하는 함수. semaphore 변수에 -1 수행
// semaphore의 값이 0일때 기다림
int
sem_wait(sem_t *sem)
{
	// critical section인 semaphore counter 값에 접근하기 위해 mutex lock 수행
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	// semaphore의 값이 0인 경우 기다리기
	if (sem->sval == 0)  {
		while (sem->sval == 0)  {
			// condition variable을 사용해 semaphore 값이 1 이상인 경우
			// 시그널을 받고 wait 상태에서 깨어남
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0)  {
				if (pthread_mutex_unlock(&sem->mutex) < 0)
					return -1;
				return -1;
			}
		}
		// semaphore를 1 감소시키기 위해 critical section에 진입
		sem->sval--;
	}
	// semaphore의 값이 1 이상인 경우
	else  {
		// semaphore를 1 감소시키기 위해 critical section에 진입
		sem->sval--;
	}

	// critical section의 사용이 끝났으므로 mutex unlock
	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

// semaphore를 할당하는 함수. semaphore 변수에 -1 수행
// semaphore 값이 0일때 기다리지 않고 -1을 반환하고 종료
int
sem_trywait(sem_t *sem)
{
	// critical section인 semaphore counter 값에 접근하기 위해 mutex lock 수행
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;
	
	// semaphore의 값이 0인 경우 -1을 반환하고 종료
	if (sem->sval == 0)  {
		if (pthread_mutex_unlock(&sem->mutex) < 0)
			return -1;
		return -1;
	}
	// semaphore의 값이 1 이상인 경우
	else  {
		// semaphore를 1 감소시키기 위해 critical section에 진입
		sem->sval--;
	}

	// critical section의 사용이 끝났으므로 mutex unlock
	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

// semaphore를 반납하는 함수. semaphore 변수에 +1 수행
int
sem_post(sem_t *sem)
{
	// critical section인 semaphore counter 값에 접근하기 위해 mutex lock 수행
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	// semaphore의 값이 0인 경우 condition variable을 사용해 시그널을 보냄
	if (sem->sval == 0)  {
		if (pthread_cond_signal(&sem->cond) < 0)  {
			if (pthread_mutex_unlock(&sem->mutex) < 0)
				return -1;
			return -1;
		}
	}

	// semaphore를 1 증가시키기 위해 critical section에 진입
	sem->sval++;

	// critical section의 사용이 끝났으므로 mutex unlock
	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

// semaphore의 현재 값을 받아오는 함수
int
sem_getvalue(sem_t *sem, int *sval)
{
	// semaphore counter 변수의 값을 받아오기
	*sval = sem->sval;

	return 0;
}

// semaphore를 제거하는 함수
int
sem_destroy(sem_t *sem)
{
	// mutex 제거
	if (pthread_mutex_destroy(&sem->mutex) < 0)
		return -1;

	// condition variable 제거
	if (pthread_cond_destroy(&sem->cond) < 0)
		return -1;

	return 0;
}
