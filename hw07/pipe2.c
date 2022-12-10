/* pipe2.c
 * 기존 pipe.c 프로그램에서 child도 parent에게 메세지를 전달하도록 추가
 * $ gcc -o pipe2 pipe2.c
 * $ ./pipe2
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define	MAX_BUF	128

main()
{
	int		n, fd[2], fd2[2];	// fd[0]은 read용, fd[1]은 write용
	pid_t	pid;
	char	buf[MAX_BUF];	// 보내고 받을 데이터를 저장할 buf

	// kernel 공간에 fd, fd2 pipe를 생성
	// pipe는 단방향 통신이므로 parent->child, child->parent 각각 생성
	if (pipe(fd) < 0)  {
		perror("pipe");
		exit(1);
	}
	if (pipe(fd2) < 0) {
		perror("pipe");
		exit(1);
	}

	// fork를 이용하여 child process 생성
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {	// child process
		// parent -> child 메세지 받기
		// child는 fd[0]을 통해 parent가 pipe에 write한 데이터를 read하므로
		// 단방향 통신을 위해 사용하지 않는 write용 fd[1]은 close
		close(fd[1]);
		printf("Child : Wait for parent to send data\n");
		// parent가 write한 데이터를 fd[0]을 통해 MAX_BUF만큼 read해오고 buf에 저장
		if ((n = read(fd[0], buf, MAX_BUF)) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		fflush(stdout);	
		write(STDOUT_FILENO, buf, n);	// 읽어온 데이터를 화면에 출력

		// child -> parent 메세지 보내기
		// child는 fd2[1]을 통해 pipe에 데이터를 write
		// 단방향 통신을 위해 사용하지 않는 read용 fd2[0]은 close
		close(fd2[0]);
		printf("Child: Send data to parent\n");
		// write할 문자열을 buf에 복사
		strcpy(buf, "Hello, World! From Child\n");
		// fd2[1]에 buf의 내용을 write.
        // 문자열의 마지막 글자인 '\0'까지 저장하기 위해 strlen(buf)+1 만큼 write
		write(fd2[1], buf, strlen(buf)+1);
	}
	else  {		// parent process
		// parent -> child 메세지 보내기
		// parent는 fd[1]을 통해 pipe에 데이터를 write하므로
		// 단방향 통신을 위해 사용하지 않는 read용 fd[0]은 close
		close(fd[0]);
		// write할 문자열을 buf에 복사
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		// fd[1]에 buf의 내용을 write. 
		// 문자열의 마지막 글자인 '\0'까지 저장하기 위해 strlen(buf)+1 만큼 write
		write(fd[1], buf, strlen(buf)+1);	
		// write 완료하면 wait 상태였던 child가 ready 상태로 바뀜

		// child -> parent 메세지 받기
		// parent는 fd2[0]을 통해 child가 pipe에 write한 데이터를 read하므로
        // 단방향 통신을 위해 사용하지 않는 write용 fd2[1]은 close
		close(fd2[1]);
		printf("Parent : Wait for child to send data\n");
		// child가 write한 데이터를 fd2[0]을 통해 MAX_BUF만큼 read해오고 buf에 저장
		if((n = read(fd2[0], buf, MAX_BUF)) < 0)  {
            perror("read");
            exit(1);
        }
		printf("Parent : Received data from child: ");
        fflush(stdout);
        write(STDOUT_FILENO, buf, n);   // 읽어온 데이터를 화면에 출력
	}

	exit(0);
}
