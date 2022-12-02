#include <stdio.h>
#include <signal.h>
#include <pwd.h>

/* nonreent.c
 * 1초마다 handler가 수행되는 프로그램
 * handler로 등록한 함수에 non-reentrant function을 사용하여 syncronization problem이 발생하게 됨
 * $ gcc -o nonreent nonreent.c (or make nonreent)
 * $ ./nonreent
 */

void
MyAlarmHandler(int signo)
{
	struct passwd	*rootptr;

	// signal handler 재등록
	signal(SIGALRM, MyAlarmHandler);
	// 1초 후에 SIGALRM 시그널을 발생시킴
	alarm(1);

	printf("in signal handler\n");

	// getpwnam 함수 내부의 pw 구조체가 root의 내용으로 바뀜
	if ((rootptr = getpwnam("root")) == NULL)  {
		perror("getpwnam");
		exit(1);
	}

	return;
}

main()
{
	struct passwd	*ptr;

	// SIGALRM에 대한 signal handler 등록
	signal(SIGALRM, MyAlarmHandler);
	// 1초 후에 SIGALRM 시그널을 발생시킴
	alarm(1);

	for ( ; ; )  {
		// getpwnam 함수 내부의 pw 구조체에 cjs에 대한 내용이 저장됨
		if ((ptr = getpwnam("cjs")) == NULL)  {
			perror("getpwnam");
			exit(1);
		}
		// 수행 도중에 SIGALRM 시그널이 발생하여 handler 함수가 수행됨

		// handler 함수 수행이 끝나고 다시 해당 코드로 돌아옴
		// 하지만 getpwnam 함수 내부의 pw 구조체의 내용이 root로 바뀐 후 이므로
		// root와 cjs를 비교하므로 strcmp의 값이 0이 아니게 됨
		if (strcmp(ptr->pw_name, "cjs") != 0)  {
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
		// syncronization problem (critical section problem) 이 발생함
		// getpwnam 함수는 reentrance function이 아니기 때문에 이러한 문제가 발생함
	}
}
