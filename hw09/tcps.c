/* tcps.c
 * tcp를 사용하여 프로세스간 통신을 구현한 서버 프로그램
 * client와 connection을 맺고 request를 처리한 후
 * 새로운 client connection을 기다리는 loop 수행
 * $ gcc -o tcps tcps.c (or make tcps)
 * $ gcc -o tcpc tcpc.c (or make tcpc)
 * $ ./tcps
 * $ ./tcpc
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"


// TCP 연결 요청을 받을 socket
// signal handler에서 사용해야하므로 전역변수로 선언
int	Sockfd;

void
CloseServer()
{	// ^C signal 발생시 socket을 close하고 프로그램 종료
	close(Sockfd);	// tcp server socket close
	printf("\nTCP Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	// client와 연결하여 메세지를 주고받을 socket
	int					newSockfd;
	// accept 시에 client socket address 정보를 담을 struct의 크기
	int					cliAddrLen;
	// client로부터 받은 request 메세지의 길이
	int 				n;
	struct sockaddr_in	cliAddr, servAddr;	// IPv4
	MsgType				msg;	// tcp.h에 정의한 MsgType struct

	signal(SIGINT, CloseServer);	// ^C signal handler 등록 

	// socket 생성			IPv4, TCP 사용
	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	// Server Socket Address 지정
	bzero((char *)&servAddr, sizeof(servAddr));	// 객체를 0으로 초기화
	servAddr.sin_family = PF_INET;	// IPv4
	// htonl/htons : network by order 4byte/2byte integer로 변환하는 함수
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소 지정. 모든 packet 받기
	servAddr.sin_port = htons(SERV_TCP_PORT);	// port number 지정

	// socket address(ip address + port number) 지정. 
	// protocol 유형별로 주소체계가 다르므로 주소에 대한 포인터와 크기를 넘김
	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	// 동시 접속 connection 수 설정. 지금은 별 의미 없음
	listen(Sockfd, 5);

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);	// client socket address의 크기
	while (1)  {
		// server에서 client와의 tcp connection을 기다림
		// newSockfd는 맺어진 tcp connection을 지칭 -> 메세지를 주고받을 때 사용
		// cliAddr에 connection을 맺은 client의 정보를 받아옴
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}
		
		// client에서 보낸 request 읽어오기
		if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
			perror("read");
			exit(1);
		}
		// client에게 받은 request 내용 출력
		printf("Received request: %s.....", msg.data);

		// reply message 생성
		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		// client에 reply 메세지 보내기 
		if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
			perror("write");
			exit(1);
		}
		printf("Replied.\n");

		usleep(10000);
		close(newSockfd);	// tcp connection을 맺은 socket close
	}	// loop의 시작으로 돌아가 새로운 connection을 기다림
}
