/* tadd.c
 * 숙제3: 추가 프로그램 작성
 * 1부터 100까지의 합을 구하는 프로그램 tadd 작성
 * 두 개의 thread를 생성하여 각각 1~50, 51~100까지의 합을 구함
 * main thread가 각 thread가 종료되기를 기다린 후 각각의 합을 더하여 출력
 * $ gcc -o tadd tadd.c -lpthread
 * $ ./tadd
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// num부터 num+49까지의 합을 구하여 호출한 thread에게 결과를 return하는 thread 함수
void
SumFunc(void* num)
{
	int		sum = 0;	// 구한 합을 저장할 지역변수
    int     st = *(int *)num;	// tid1에서는 1의 값이, tid2에서는 51의 값이 저장됨
	int*	ret = (int*)malloc(sizeof(int));	// dynamic allocation. heap memeory에 저장됨

	for (int i=0; i < 50; i++) {
		sum += st+i;	// 지역변수로 sum을 구함
	}
	*ret = sum;	// ret이 가리키는 heap memeory 공간에 sum의 값을 복사
	pthread_exit((void *) ret);	// int* ret을 return
}

main()
{
	pthread_t	tid1, tid2;		// 생성한 thread의 id를 저장할 변수
	int			*ret1, *ret2;	// thread별로 수행한 함수의 결과값을 저장할 변수
	int			arg1 = 1;		// tid1의 start value
	int			arg2 = 51;		// tid2의 start value

	// Thread ID: tid1, Thread function: SumFunc, Thread argument: arg1
	if (pthread_create(&tid1, NULL, (void *) SumFunc, (void *)&arg1) < 0) {
		perror("pthread_create");	// 오류 메세지 출력
		exit(1);
	}

	// Thread ID: tid2, Thread function: SumFunc, Thread argument: arg2
	if (pthread_create(&tid2, NULL, (void *) SumFunc, (void *)&arg2) < 0) {
        perror("pthread_create");	// 오류 메세지 출력
        exit(1);
    }

	// tid1이 종료되길 기다림. ret1에 함수에서 반환한 동적할당 포인터가 들어감
	// void** <-> int* 형변환 수행
	if (pthread_join(tid1, (void**)&ret1) < 0) {
		perror("pthread_join");
		exit(1);
	}
	
	// tid2가 종료되길 기다림. ret2에 함수에서 반환한 동적할당 포인터가 들어감
    // void** <-> int* 형변환 수행
	if (pthread_join(tid2, (void**)&ret2) < 0) {
        perror("pthread_join");
        exit(1);
    }

	printf("%d\n", *ret1 + *ret2);	// tid1과 tid2의 수행결과를 합하여 최종 결과를 출력: 5050
}

