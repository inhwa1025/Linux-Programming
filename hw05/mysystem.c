/* mysystem.c
 * 숙제2: 추가 프로그램 작성
 * int mysystem(char *cmd) 함수 구현
 * 동작은 "system" system call과 동일해야 함
 * fork, execv, waitpid system call을 이용하여 구현
 * $ gcc -o mysystem mysystem.c (make mysystem)
 * $ ./mysystem
 */
#include <stdio.h>
#include <sys/types.h>

#define	MAX_CMD		256


int 
mysystem(char *cmd)
{
	pid_t		pid;	// 생성한 child process의 process id를 저장할 변수
	int 	ret = 0;	// child process의 exit code를 저장할 변수
	char *argv[] = {"sh", "-c", cmd, NULL};		// execv에 넘길 argument의 vector 선언
	
	if ((pid = fork()) < 0) {	// child process를 생성하고 process id를 pid에 저장
		perror("fork");	// 오류 메세지 출력
		exit(1);
	}
	else if (pid == 0) {	// child process는 입력받은 command를 수행하고 종료
		/* child */
		if (execv("/bin/sh", argv) < 0) {
			perror("execv");	// 오류 메세지 출력
			exit(1);
		}
	}
	else {
		/* parent */
		if (waitpid(pid, &ret, 0) < 0) {	// child process가 종료되길 기다림
			perror("waitpid");	// 오류 메세지 출력
			return ret;	// child process의 exit code를 return
		}
	}
}

main()
{
	char	cmd[MAX_CMD];	// 사용자에게 입력받은 command를 저장할 문자열 변수
	int		status;		// mysystem 프로그램의 종료 status를 받아와 저장할 변수

	printf("CMD> ");
	fgets(cmd, MAX_CMD, stdin);	// 사용자에게서 command를 입력을 받음

	if ((status = mysystem(cmd)) < 0) {	// mysystem 프로그램 실행
		perror("mysystem");	// 오류 메세지 출력
		exit(1);
	}
	printf("exit status = %d\n", status);	// mysystem 프로그램의 종료 status를 출력
}

