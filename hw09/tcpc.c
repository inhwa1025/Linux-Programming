/* tcps.c
 * tcp를 사용하여 프로세스간 통신을 구현한 클라이언트 프로그램
 * server와 connection을 맺고 request을 보냄
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
#include "tcp.h"


main(int argc, char *argv[])
{
	// server와 tcp connection을 맺어 request를 보내기 위한 socket
	int					sockfd;
	// server로부터 받은 reply message의 길이
	int					n;
	struct sockaddr_in	servAddr;	// IPv4
	MsgType				msg;	// tcp.h에 정의한 MsgType struct

	// socket 생성			IPv4, TCP 사용
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	// 연결할 server의 socket address 설정
	bzero((char *)&servAddr, sizeof(servAddr));	// 객체를 0으로 초기화
	servAddr.sin_family = PF_INET;	// IPv4
	// inet_addr: IP 주소를 network by order 4byte integer로 변환
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);	// IP 주소 지정
	servAddr.sin_port = htons(SERV_TCP_PORT);	// port number 지정

	// tcp server와 connection 맺기
	if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)  {
		perror("connect");
		exit(1);
	}
	// connection 요청 -> TCP 3-way handshaking 발생 (Client에서 SYN packet 전송)
	// -> Server에서 ACK packet 전송 -> Client에서 ACK packet 전송
	// -> tcp connection이 맺어짐

	// request message 생성
	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	// server에 request 메세지 보내기
	if (write(sockfd, (char *)&msg, sizeof(msg)) < 0)  {
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");
	
	// server에서 보낸 reply 메세지 읽어오기
	if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}
	// server에게서 받은 reply 메세지 내용 출력
	printf("Received reply: %s\n", msg.data);

	close(sockfd);	// tcp connection을 맺은 socket close
}
