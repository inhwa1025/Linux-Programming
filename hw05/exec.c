#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/* exec.c
 * 프로그램 안에서 다른 프로그램을 실행하는 exec system call을 사용하는 프로그램
 * $ gcc -o exec exec.c (or make exec)
 * $ ./exec
 */
main()
{
	pid_t	pid;	// 생성한 child process의 process id를 저장할 변수
	// 실행할 프로그램들의 목록을 저장한 vector 선언
	char	*argv[] = { "mycp", "exec.c", "exec2.c", NULL };

	// execl: 실행할 프로그램의 argument들을 list로 넘김
	if ((pid = fork()) < 0)  {	// child process를 생성하고 process id를 pid에 저장
		perror("fork");		// 오류 메세지 출력
		exit(1);
	}
	else if (pid == 0)  {	// child process -> execl을 통해 mycp 프로그램 실행
		// mycp exec.c exec2.c
		if (execl("/home/lsp63/hw03/mycp.c", "mycp", "exec.c", "exec2.c", NULL) < 0)  {
			perror("execl");	// 오류 메세지 출력
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {	// child process가 끝날때까지 wait
		perror("waitpid");	// 오류 메세지 출력
		exit(1);
	}

	// execv: 실행할 프로그램의 argument들을 vector로 넘김
	if ((pid = fork()) < 0)  {	// child process를 생성하고 process id를 pid에 저장
		perror("fork");		// 오류 메세지 출력
		exit(1);
	}
	else if (pid == 0)  {	// child process -> execv을 통해 mycp 프로그램 실행
		// mycp exec.c exec2.c
		if (execv("/home/lsp63/hw03/mycp.c", argv) < 0)  {
			perror("execv");	// 오류 메세지 출력
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {	// child process가 끝날때까지 wait
		perror("waitpid");	// 오류 메세지 출력
		exit(1);
	}

	// execlp: 실행파일의 전체 경로가 아닌 이름만 넘기면 됨. argument들은 List로 넘김
	if ((pid = fork()) < 0)  {	// child process를 생성하고 process id를 pid에 저장
		perror("fork");		// 오류 메세지 출력
		exit(1);
	}
	else if (pid == 0)  {	// child process -> PATH 환경 변수를 통해 env 프로그램 실행
		// execute 'env' from PATH
		if (execlp("env", "env", NULL) < 0)  {
			perror("execlp");	// 오류 메세지 출력
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {	// child process가 끝날때까지 wait
		perror("waitpid");	// 오류 메세지 출력
		exit(1);
	}
}

