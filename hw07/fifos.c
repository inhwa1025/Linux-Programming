/* fifos.c
 * client와 server가 communication을 하기 위해 FIFO를 사용한 프로그램
 * 해당 파일은 server에 해당함. client는 fifoc.c에 구현됨
 * $ gcc -o fifos fifos.c (or make fifos)
 * $ gcc -o fifoc fifoc.c (or make fifoc)
 * $ ./fifos
 * $ ./fifoc
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"


// fifos 프로그램이 종료되기 전에 생성된 FIFO file을 삭제하고 종료하기 위한 handler
void
SigIntHandler(int signo)
{
	// FIFO file remove
	if (remove(SERV_FIFO) < 0)  {
		perror("remove");
		exit(1);
	}

	exit(0);
}


main()
{
	int			fd, cfd, n;
	MsgType		msg;

	// ^C가 입력되면 FIFO file을 지우고 종료되도록 signal handler 등록
	if (signal(SIGINT, SigIntHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	// owner read/write 권한으로 FIFO file 생성
	if (mkfifo(SERV_FIFO, 0600) < 0)  {
		// 이미 존재하는 경우라면 그대로 사용하면 되므로 예외 처리
		if (errno != EEXIST)  { 
			perror("mkfifo");
			exit(1);
		}
	}

	// special file (FIFO file)을 read/write용으로 open
	if ((fd = open(SERV_FIFO, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

	// client에게서 메세지를 계속 받을 수 있도록 loop를 돌며 계속 대기
	while (1)  {
		// fd (Well-known FIFO file)에서 client가 보낸 메세지를 read
		if ((n = read(fd, (char*)&msg, sizeof(msg))) < 0)  {
			// EINTR은 interrupt signal
			// client에서 write 수행 후 interrupt가 발생되는데
			// 이는 오류가 아니므로 예외처리
			if (errno == EINTR)  {
				continue;
			}
			else  {
				perror("read");
				exit(1);
			}
		}
		// client에게서 받아온 메세지를 출력
		printf("Received request: %s.....", msg.data);

		// client에게 메세지를 보내기 위한 FIFO file을 read 전용으로 open
		if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0)  {
			perror("open");
			exit(1);
		}
		// 메세지를 보낸 후에 자신의 process id와 함께 보냈다는 문구 출력
		sprintf(msg.data, "This is a reply from %d.", getpid());
		// cfd (Client-specific FIFO file)에 client에게 보낼 메세지를 write
		write(cfd, (char*)&msg, sizeof(msg));
		// 파일 사용이 끝났으므로 file을 close
		close(cfd);
		printf("Replied.\n");
	}
}
