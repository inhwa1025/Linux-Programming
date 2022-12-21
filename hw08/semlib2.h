// mutex와 condition variable을 사용하여 POSIX semaphore를 구현한 것
// $ gcc -c semlib.c (or make semlib2.o)
#include <pthread.h>

typedef struct  {
	int		sval;	// semaphore counter 변수 -> critical section
	pthread_mutex_t	mutex;	// mutex
	pthread_cond_t	cond;	// condition variable
}
	sem_t;

int		sem_init(sem_t *sem, int pshared, int value);	// semaphore 생성
int		sem_wait(sem_t *sem);	// semaphore 할당(-1 수행). 0이면 기다리기
int		sem_trywait(sem_t *sem); // semaphore 할당(-1 수행). 0일때 기다리지 않음
int		sem_post(sem_t *sem);	// semaphore 반납(+1 수행)
int		sem_getvalue(sem_t *sem, int *sval);	// semaphore의 현재값 받아오기
int		sem_destroy(sem_t *sem);	// semaphore 제거
