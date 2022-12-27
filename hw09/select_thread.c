/* select_thread.c
 * Synchronous I/O multiplexing을 수행하는 기존 select 프로그램을
 * thread를 생성하여 처리하도록 변경한 프로그램
 * select system call을 사용해 여러 요청을 동기적으로 처리
 * 모든 유형의 요청(tcp, udp)을 처리 가능
 * $ gcc -o select_thread select_thread.c -lpthread
 * $ ./select_thread
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "select.h"
#include <pthread.h>


// server에서 사용하는 socket address
int	TcpSockfd;	// tcp
int	UdpSockfd;	// udp
int	UcoSockfd;	// unix domain connection-oriented
int	UclSockfd;	// unix domain connection-less


// signal handler
void
CloseServer()
{
	// 생성한 socket들을 일괄 close
	close(TcpSockfd);
	close(UdpSockfd);
	close(UcoSockfd);
	close(UclSockfd);
	// fifo remove
	if (remove(UNIX_STR_PATH) < 0) {
		perror("remove");
	}
	if (remove(UNIX_DG_PATH) < 0) {
		perror("remove");
	}

	printf("\nServer daemon exit.....\n");

	exit(0);
}

// create tcp socket
void
MakeTcpSocket()
{
	struct sockaddr_in	servAddr;

	if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(TcpSockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	listen(TcpSockfd, 5);
}

// create udp socket
void
MakeUdpSocket()
{
	struct sockaddr_in	servAddr;

	if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_UDP_PORT);

	if (bind(UdpSockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}
}

// create unix domain connection-oriented socket
void
MakeUcoSocket()
{
	struct sockaddr_un	servAddr;
	int					servAddrLen;

	if ((UcoSockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_STR_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	if (bind(UcoSockfd, (struct sockaddr *)&servAddr, servAddrLen) < 0) {
		perror("bind");
		exit(1);
	}

	listen(UcoSockfd, 5);
}

// create unix domain connection-less socket
void
MakeUclSocket()
{
	struct sockaddr_un	servAddr;
	int					servAddrLen;

	if ((UclSockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_DG_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	if (bind(UclSockfd, (struct sockaddr *)&servAddr, servAddrLen) < 0) {
		perror("bind");
		exit(1);
	}
}

// tcp connection을 맺고 request를 처리하여 reply를 보냄
void
ProcessTcpRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	// connection accept -> new socket
	newSockfd = accept(TcpSockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
	if (newSockfd < 0) {
		perror("accept");
		exit(1);
	}

	// request receive
	if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}
	printf("Received TCP request: %s.....", msg.data);

	// send reply
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from pid %d, threadid %ld.", getpid(), pthread_self());
	if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0) {
		perror("write");
		exit(1);
	}
	printf("Replied.\nthread %ld terminated.\n",pthread_self());

	// conncection socket close
	close(newSockfd);
	pthread_exit(NULL);	// thread 종료
}

// udp request를 처리하고 reply를 보냄
void
ProcessUdpRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

    // receive request
	cliAddrLen = sizeof(cliAddr);
	if ((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg),
						0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0) {
		perror("recvfrom");
		exit(1);
	}
	printf("Received UDP request: %s.....", msg.data);

	// send reply
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from pid %d, threadid %ld.", getpid(),pthread_self());
	if (sendto(UdpSockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0) {
		perror("sendto");
		exit(1);
	}
	printf("Replied.\nthread %ld exit.\n", pthread_self());
	pthread_exit(NULL);
}

// unix domain connection-oriented와 connection을 맺고 request를 처리하여 reply를 보냄
// 사실상 fifo와 동일하다
void
ProcessUcoRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_un	cliAddr;
	MsgType				msg;

	// connection accept -> new socket
	cliAddrLen = sizeof(cliAddr);
	newSockfd = accept(UcoSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
	if (newSockfd < 0)  {
		perror("accept");
		exit(1);
	}

	// receive request
	if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}
	printf("Received UNIX-domain CO request: %s.....", msg.data);

	// send reply
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from pid %d, threadid %ld.", getpid(), pthread_self());
	if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
		perror("write");
		exit(1);
	}
	printf("Replied.\nthread %ld terminated.\n",pthread_self());

	// connection socket close
	close(newSockfd);
	pthread_exit(NULL);
}

// unix domain connection-less의 request를 처리하고 reply를 보냄
// 사실상 fifo와 동일하다
void
ProcessUclRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_un	cliAddr;
	MsgType				msg;

	// receive request
	cliAddrLen = sizeof(cliAddr);
	if ((n = recvfrom(UclSockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
		perror("recvfrom");
		exit(1);
	}
	printf("Received UNIX-domain CL request: %s.....", msg.data);

	// send reply
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from pid %d, threadid %ld.", getpid(), pthread_self());
	if (sendto(UclSockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
		perror("sendto");
		exit(1);
	}
	printf("Replied.\nthread %ld exit.\n", pthread_self());
	pthread_exit(NULL);
}

main (int argc, char *argv[])
{
	fd_set		fdvar;
	int			count;
	pthread_t	tid;

	signal(SIGINT, CloseServer);	// signal handler 등록

	// tcp, udp, unix domain socket 생성
	MakeTcpSocket();
	MakeUdpSocket();
	MakeUcoSocket();
	MakeUclSocket();

	printf("Server daemon started.....\n");

	while (1) {
		// 기다릴 socket의 list 설정. bit field를 켜고 끄는 연산
		FD_ZERO(&fdvar);	// 0으로 초기화
		FD_SET(TcpSockfd, &fdvar);	// TcpSockfd에 대한 비트 켜기
		FD_SET(UdpSockfd, &fdvar);	// UdpSockfd에 대한 비트 켜기
		FD_SET(UcoSockfd, &fdvar);	// UcoSockfd에 대한 비트 켜기
		FD_SET(UclSockfd, &fdvar);	// UclSockfd에 대한 비트 켜기

		// socket 연결 또는 request가 오길 기다림. read만 수행. write/exception은 X
		if ((count = select(10, &fdvar, (fd_set *)NULL, (fd_set *)NULL,
							(struct timeval *)NULL)) < 0) {
			perror("select");
			exit(1);
		}

		// 어떤 socket에서 request가 왔는지 확인 (FD_ISSET으로 fdvar를 확인)
		while (count--) {
			// tcp socket에서 온 것인지 확인
			if (FD_ISSET(TcpSockfd, &fdvar)) {
				if (pthread_create(&tid, NULL, (void*)ProcessTcpRequest, NULL) < 0) {
					perror("pthread_create");
					exit(1);
				}
			}
			// udp socket에서 온 것인지 확인
			else if (FD_ISSET(UdpSockfd, &fdvar)) {
				if (pthread_create(&tid, NULL, (void*)ProcessUdpRequest, NULL) < 0) {
					perror("pthread_create");
					exit(1);
				}
			}
			// udp socket에서 온 것인지 확인
			else if (FD_ISSET(UdpSockfd, &fdvar))  {
				if (pthread_create(&tid, NULL, (void*)ProcessUcoRequest, NULL) < 0) {
                     perror("pthread_create");
					 exit(1);
				}
			}
			// unix domain connection-less socket에서 온 것인지 확인
			else if (FD_ISSET(UclSockfd, &fdvar))  {
				if (pthread_create(&tid, NULL, (void*)ProcessUclRequest, NULL) < 0) {
					perror("pthread_create");
					exit(1);
				}
			}
		}
	}
}

