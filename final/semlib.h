// POSIX semaphore-like library using System V semaphore
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int		semInit(key_t key);	// 생성 또는 받아오기
int		semInitValue(int semid, int value);	// 초기화
int		semWait(int semid);	// 할당. 0일때 wait
int		semTryWait(int semid);	// 할당. 0일때 기다리지 않고 false return
int		semPost(int semid);	// 반납
int		semGetValue(int semid);	// 현재 값 받아오기
int		semDestroy(int semid);	// 삭제
