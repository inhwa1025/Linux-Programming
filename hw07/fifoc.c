/* fifos.c
 * client와 server가 communication을 하기 위해 FIFO를 사용한 프로그램
 * 해당 파일은 client에 해당함. server는 fifos.c에 구현됨
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
#include <errno.h>
#include "fifo.h"


main()
{
	char		fname[MAX_FIFO_NAME];	// client-specific FIFO file name
	int			fd, sfd, n;	// client-specific FIFO, well-known(server) FIFO,  메세지 길이
	MsgType		msg;	// 주고받을 메세지를 저장할 변수

	sprintf(fname, ".fifo%d", getpid());	// client-specific FIFO name 지정
	// owner read/write 권한으로 FIFO file 생성
	if (mkfifo(fname, 0600) < 0)  {
		perror("mkfifo");
		exit(1);
	}

	// client-specific FIFO file을 read/write용으로 open
	if ((fd = open(fname, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}
	// well-known(server) FIFO file을 read/write용으로 open
	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

	strcpy(msg.returnFifo, fname);
	sprintf(msg.data, "This is a request from %d.", getpid());
	write(sfd, (char*)&msg, sizeof(msg));	// server FIFO에 메세지 전달
	printf("Sent a request.....");

	// client-specific FIFO에 write된 (server가 보낸) 메세지를 read
	if ((n = read(fd, (char*)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}

	// server에게서 받아온 메세지를 출력
	printf("Received reply: %s\n", msg.data);

	// 2개의 FIFO file의 사용이 모두 끝났으므로 file을 close
	close(fd);
	close(sfd);

	// 생성했던 client-specific FIFO file을 remove
	/* Remove temporary FIFO */
	if (remove(fname) < 0)  {
		perror("remove");
		exit(1);
	}
}
