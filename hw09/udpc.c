/* udps.c
 * udp 프로토콜을 사용하여 프로세스간 통신을 구현한 클라이언트 프로그램
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
#include "udp.h"


main(int argc, char *argv[])
{
	// udp 통신을 수행할 client socket
	int					sockfd;
	// server로부터 받은 reply 메세지의 길이
	int					n;
	// server socket address의 정보를 담을 struct의 크기
	int					peerAddrLen;
	struct sockaddr_in	servAddr, peerAddr;	// socket address. IPv4
	MsgType				msg;	// udp.h에 정의한 MsgType struct

	// socket 생성			IPv4, UDP 사용
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	// Server Socket Address 지정
	bzero((char *)&servAddr, sizeof(servAddr));	// 0으로 초기화
	servAddr.sin_family = PF_INET;	// IPv4
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);	// IP 주소 지정
	servAddr.sin_port = htons(SERV_UDP_PORT);	// port number 지정

	// request message 생성
	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	// server에 request 메세지 보내기
	if (sendto(sockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)  {
		perror("sendto");
		exit(1);
	}
	printf("Sent a request.....");
	
	// reply를 보낸 server socket address의 크기
	peerAddrLen = sizeof(peerAddr);
	if ((n = recvfrom(sockfd, (char *)&msg, sizeof(msg),
						0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0)  {
		perror("recvfrom");
		exit(1);
	}
	// server로부터 받은 reply 메세지 출력
	printf("Received reply: %s\n", msg.data);

	close(sockfd);	// socket close
}
