/* semlib.c
 * POSIX semaphore-like library using System V semaphore
 * System V semaphore를 사용하여 POSIX semaphore처럼 구현해놓은 것
 * gcc -c semlib.c (or make semlib.o)
 */
#include <stdio.h>
#include "semlib.h"
 

// semaphore 생성 또는 존재 시 id 받아오기
int
semInit(key_t key)
{
    int     semid;

	// key를 통해 physical memory에 semaphore 할당
    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  {
		perror("semget");
        return -1;
    }

    return semid;
}

// semaphore 값 초기화
int
semInitValue(int semid, int value)
{
    union semun {
            int     val;
    } semun;

    semun.val = value;
	// 입력한 값 value로 semaphore의 값을 초기화
    if (semctl(semid, 0, SETVAL, semun) < 0)  {
		perror("semctl");
        return -1;
    }
 
    return semid;
}

// semaphore 할당. 값이 0이면 wait.
// 0이 아니면 -1을 하고 critical section에 접근
int
semWait(int semid)
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
	// SEM_UNDO: 프로세스 비정상적 종료 시 semaphore 동작을 rollback
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

// semaphore 할당. 값이 0이면 wait하지 않고 오류 반환
// 0이 아니면 -1을 하고 critical section에 접근
int
semTryWait(int semid)
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
	// IPC_NOWAIT: counter가 0이면 기다리지 않고 오류를 반환
	// SEM_UNDO: 프로세스 비정상적 종료 시 semaphore 동작을 rollback
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

// semaphore 반환. +1을 하고 critical section에서 나옴
int
semPost(int semid)
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = 1;
	// SEM_UNDO: 프로세스 비정상적 종료 시 semaphore 동작을 rollback
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

// semaphore의 현재 counter 값을 반환
int
semGetValue(int semid)
{
    union semun {
            int     val;
    } dummy;
 
    return semctl(semid, 0, GETVAL, dummy);
}

// semaphore 삭제
int
semDestroy(int semid)
{
    union semun {
            int     val;
    } dummy;
 
    if (semctl(semid, 0, IPC_RMID, dummy) < 0)  {
		perror("semctl");
        return -1;
    }
    close(semid);
 
    return 0;
}
