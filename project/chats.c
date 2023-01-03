/* chats.c
 * thread를 생성하지 않고 select system call로 multiplexing 처리
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "chat.h"

#define	DEBUG

#define	MAX_CLIENT	5
#define	MAX_ID		32
#define	MAX_BUF		256

typedef	struct  {
	int			sockfd;	// client와 통신하는 socket
	int			inUse;	// 1: 해당 인덱스가 사용중, 0: 사용중이지 않음
	char		uid[MAX_ID];	// client id
}
	ClientType;

int				Sockfd;	// server socket

ClientType		Client[MAX_CLIENT];


// 사용하지 않는 ID를 가져오고 inUse를 1로 변경
int
GetID()
{
	int	i;

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (! Client[i].inUse)  {
			Client[i].inUse = 1;	// 사용중
			return i;
		}
	}
}

// broadcasting
void
SendToOtherClients(int id, char *buf)
{
	int		i;
	char	msg[MAX_BUF+MAX_ID];

	sprintf(msg, "%s> %s", Client[id].uid, buf);
#ifdef	DEBUG
	printf("%s", msg);
	fflush(stdout);
#endif

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse && (i != id))  {
			if (send(Client[i].sockfd, msg, strlen(msg)+1, 0) < 0)  {
				perror("send");
				exit(1);
			}
		}
	}
}
	

void
ProcessClient()
{
	char				buf[MAX_BUF];
	int					newSockfd, id, count, n, cliAddrLen;
	fd_set				fdset;
	struct sockaddr_in	cliAddr;
	
	cliAddrLen = sizeof(cliAddr);

	while (1) {
		FD_ZERO(&fdset);
		FD_SET(Sockfd, &fdset);
		for (id = 0; id < MAX_CLIENT; id++) {
			if (Client[id].inUse) {
				FD_SET(Client[id].sockfd, &fdset);
			}
		}

		if ((count = select(10, &fdset, (fd_set *)NULL, (fd_set *)NULL,
							(struct timeval *)NULL)) < 0) {
			perror("select");
			exit(1);
		}
		
		while (count--) {
			if (FD_ISSET(Sockfd, &fdset)) {	// tcp conncection 요청인 경우
				// connection accept -> newSockfd 생성
				newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
				if (newSockfd < 0) {
					perror("accept");
					exit(1);
				}

				id = GetID();	// 사용하지 않는 ID 하나 받아오기
				Client[id].sockfd = newSockfd;	// client와 통신에 사용할 소켓 지정

				// 수신한 첫 메세지는 client의 id 정보
				if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0) {
					perror("recv");
					exit(1);
				}
				printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);
			}
			else {	// 메세지 수신의 경우
				for (id = 0; id < MAX_CLIENT; id++) {
					if (Client[id].inUse == 0) {
						continue;
					}
					// client로부터 메세지를 받은 경우
					if (FD_ISSET(Client[id].sockfd, &fdset)) {
						// broadcast
						if ((n = recv(Client[id].sockfd, buf, MAX_BUF, 0)) < 0) {
							perror("recv");
							exit(1);
						}
						if (n == 0) {	// 연결 종료 신호인 경우
							printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);
							Client[id].inUse = 0;
							
							// select가 더 이상 메세지를 기다리지 않도록
							FD_CLR(Client[id].sockfd, &fdset);
							// socket close
							close(Client[id].sockfd);

							strcpy(buf, "log-out.....\n");
							SendToOtherClients(id, buf);
						}
						else {	// 메세지인 경우 broadcast
							SendToOtherClients(id, buf);
						}
					}
				}
			}
		}
	}
}


void
CloseServer(int signo)
{
	int		i;

	close(Sockfd);

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse)  {
			close(Client[i].sockfd);
		}
	}

	printf("\nChat server terminated.....\n");

	exit(0);
}

main(int argc, char *argv[])
{
	int					one = 1;
	struct sockaddr_in	servAddr;

	signal(SIGINT, CloseServer);	// signal handler

	// socket 생성
	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	// override option
	if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)  {
		perror("setsockopt");
		exit(1);
	}

	// socket config
	bzero((char *)&servAddr, sizeof(servAddr));	// 0으로 초기화
	servAddr.sin_family = PF_INET;	// IPv4
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소 설정. 모든 패킷 수신
	servAddr.sin_port = htons(SERV_TCP_PORT);	// port number 설정

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);

	printf("Chat server started.....\n");

	ProcessClient();
}
