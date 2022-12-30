/* udps.c
 * udp 프로토콜을 사용하여 프로세스간 통신을 구현한 서버 프로그램
 * connection less이므로 별도의 연결을 맺는 것 없이 메세지를 주고받음
 * request를 기다리고 처리하고 또다시 기다리는 loop 수행
 * $ gcc -o udps udps.c (or make udps)
 * $ gcc -o udpc udpc.c (or make udpc)
 * $ ./udps
 * $ ./udpc
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "udp.h"


// udp 통신을 수행할 socket
// signal handler에서 사용해야하므로 전역변수로 선언
int	Sockfd;

void
CloseServer()
{	// ^C signal 발생시 socket을 close하고 프로그램 종료
	close(Sockfd);	// udp server socket close
	printf("\nUDP Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	// client socket address의 정보를 담을 struct의 크기
	int					cliAddrLen;
	// client로부터 받은 request 메세지의 길이
	int 				n;
	struct sockaddr_in	cliAddr, servAddr;	// IPv4
	MsgType				msg;	// udp.h에 정의한 MsgType struct

	signal(SIGINT, CloseServer);	// ^C signal handler 등록 

	// socket 생성			IPv4, UDP 사용
	if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	// Server Socket Address 지정
	bzero((char *)&servAddr, sizeof(servAddr));	// 0으로 초기화
	servAddr.sin_family = PF_INET;	// IPv4
	// htonl/htons : network by order 4byte/2byte integer로 변환하는 함수
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소 지정. 모든 packet 받기
	servAddr.sin_port = htons(SERV_UDP_PORT);	// port number 지정

	// socket address(ip address + port number) 지정. 
	// protocol 유형별로 주소체계가 다르므로 주소에 대한 포인터와 크기를 넘김
	if (bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	printf("UDP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);	// client socket address의 크기
	while (1)  {
		// client로부터 request 메세지 받기
		// cliAddr에 request를 보낸 client의 정보를 받아옴 -> sendto 시에 사용
		if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg),
						0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);

		// reply message 생성
		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		// client에 reply 메세지 보내기 
		if (sendto(Sockfd, (char *)&msg, sizeof(msg),
					0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
}
