/* mymore.c
 * command에서 'ls -l | more'을 수행한 것과 똑같이 수행되도록 
 * pipe와 dup system call을 사용해 구현한 프로그램
 * $ gcc -o mymore mymore.c (or make mymore)
 * $ ./mymore
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int 
main() 
{
	int fd[2];	// fd[0]은 read용, fd[1]은 write용
	pid_t child;

	// pipe 생성
	if(pipe(fd) < 0)  {
		perror("pipe");
		exit(1);
	}
 
	child = fork();	// child process 생성
	if(child == (pid_t)(-1))  {
        perror("fork");
		exit(1);
	}
	else if(child == (pid_t)0) {	// child process
		// 출력(1에 해당)을 duplicate할 것이므로 1번 file descriptor를 close
		close(1);
		close(fd[0]);	// write할 것이므로 read에 해당하는 fd[0]은 close
        
		// fd[1]과 닫혀있는 1번 file descriptor를 duplicate
        if(dup(fd[1]) < 0)  {
        	perror("dup");
			exit(1);
		}

		// 환경변수를 사용하여 "ls -l" 프로그램을 실행
        if((execlp("ls", "ls", "-l", NULL)) == -1)  {
        	perror("execlp");
			exit(1);
		}
	} 
	else {	// parent process
       	// 입력(0에 해당)을 duplicate할 것이므로 0번 file descriptor를 close
		close(0);
		close(fd[1]);	// read할 것이므로 write에 해당하는 fd[1]은 close
        
		// fd[0]과 0번 file descriptor를 duplicate하도록 지정
       	if(dup2(fd[0], 0) < 0)  {
       		perror("dup");
			exit(1);
		}

		// 환경변수를 사용하여 "more" 프로그램을 실행
       	if((execlp("more", "more", NULL)) == -1)  {
       		perror("execlp");
			exit(1);
		}
	}
 
	return 0;
}
