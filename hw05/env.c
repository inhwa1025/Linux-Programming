#include <stdio.h>

/* env.c
 * enviroment variable에 접근하여 그 값을 출력하는 프로그램
 * main 함수의 parameter 중 *envp[]는 환경변수들을 담은 배열이다
 * 환경변수를 읽어오는 두 가지 방법
 * 	1. main의 3번째 parameter로 char *envp[]를 사용하여 환경변수의 배열 가져오기
 * 	2. 외부 변수인 extern char **environ을 선언하여 환경변수에 접근
 * $ gcc -o env env.c (or make env)
 * $ ./env
 */
main(int argc, char *argv[], char *envp[])
{
	int			i;	// iterator index로 사용할 integer 변수 선언
	char		**p;	// iterator index로 사용할 이중 포인터 변수 선언
	extern char	**environ;	// C start-up routin의 환경변수를 담은 전역변수를 extern으로 가져와 선언

	printf("List command-line arguments\n");
	// command line에서 입력한 argument들을 반복문을 통해 한 줄씩 출력
	for (i = 0 ; i < argc ; i++)  {
		printf("%s\n", argv[i]);
	}

	printf("\n");
	printf("List environment variables from environ variable\n");
	// 환경변수들을 한 줄씩 출력 - 2번 방법 사용
#if 1
	for (i = 0 ; environ[i] != NULL ; i++)  {
		printf("%s\n", environ[i]);
	}
#else
	for (p = environ ; *p != NULL ; p++)  {
		printf("%s\n", *p);
	}
#endif

	printf("\n");
	printf("List environment variables from envp variable\n");
	// 환경 변수들을 한 줄씩 출력 - 1번 방법 사용
#if 1
	for (i = 0 ; envp[i] != NULL ; i++)  {
		printf("%s\n", envp[i]);
	}
#else
	for (p = envp ; *p != NULL ; p++)  {
		printf("%s\n", *p);
	}
#endif
}

