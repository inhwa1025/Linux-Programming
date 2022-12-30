/* tcps_process.c
 * 기존 tcps 프로그램을 connection이 맺어진 후 
 * child process를 생성하여 전담처리하도록 변경
 * $ gcc -o tcps_process tcps_process.c
 * $ gcc -o tcpc tcpc.c (or make tcpc)
 * $ ./tcps_process
 * $ ./tcpc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"

// TCP 연결 요청을 받을 socket
// signal handler에서 사용해야하므로 전역변수로 선언
int Sockfd;

void
CloseServer()
{
	// ^C signal 발생시 socket을 close하고 프로그램 종료
	close(Sockfd);	// tcp server socket close
	printf("\nTCP Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;
	MsgType 			msg;
	pid_t pid;

	signal(SIGINT, CloseServer);	// signal handler 등록 

	// socket 생성			IPv4, TCP 사용
	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	
	// Server Socket Address 지정
	bzero((char *)&servAddr, sizeof(servAddr));	// 0으로 초기화
	servAddr.sin_family = PF_INET;	// IPv4
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소 지정. 모든 패킷 받기
	servAddr.sin_port = htons(SERV_TCP_PORT);	// port number 지정

	// socket address(ip address + port number) 지정
	if (bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	// connection 수 설정. 지금은 별 의미 없음
	listen(Sockfd, 5);

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1) {
		// tcp connection을 맺어 새로운 소켓에 저장
		newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
		if (newSockfd < 0) {
			perror("accept");
			exit(1);
		}

		// tcp connection의request를 처리할 child process 생성
		if ((pid = fork()) < 0) {
			perror("fork");
			exit(1);
		}
		else if (pid == 0) {	// child process
			// client에서 보낸 request 읽어오기
			if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0) {
				perror("read");
				exit(1);
			}
			// client에게 받은 request 내용 출력
			printf("Received request: %s.....", msg.data);

			// reply message 생성
			msg.type = MSG_REPLY;
			sprintf(msg.data, "This is a reply from %d.", getpid());
			// client에 reply 메세지 보내기
			if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0) {
				perror("write");
				exit(1);
			}
			printf("Replied.\n");

			usleep(10000);
			close(newSockfd);	// tcp connection을 맺은 socket close
			printf("child process %d terminated.\n", getpid());
			exit(0);
		}
		else {	// parent process
			// 아무것도 수행하지 않고 다음 요청을 받음
		}
	}
}

