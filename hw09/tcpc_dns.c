/* tcps_dns.c
 * tcp를 사용하여 프로세스간 통신을 구현한 클라이언트 프로그램
 * server의 ip주소뿐만 아니라 hostname을 이용해 connection을 맺음
 * 입력 argument로 domain name 또는 dotted decimal ip 주소를 받음
 * $ gcc -o tcps tcps.c (or make tcps)
 * $ gcc -o tcpc_dns tcpc_dns.c (or make tcpc_dns)
 * $ ./tcps
 * $ ./tcpc_dns 127.0.0.1
 * $ ./tcpc_dns 163.180.140.216
 * $ ./tcpc_dns celinux2.khu.ac.kr
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcp.h"


main(int argc, char *argv[])
{
	// server와 tcp connection을 맺어 request를 보내기 위한 socket
	int					sockfd, n;
	struct sockaddr_in	servAddr;	// socket address. IPv4
	MsgType				msg;	// tcp.h에 정의한 MsgType struct
	// domain name을 통한 연결 시 ip 주소로 변환하여 저장
	struct hostent		*hp;	

	// domain name 또는 dotted decimal ip 주소를 입력받았는지 확인
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
		exit(1);
	}

	// socket 생성			IPv4, TCP 사용
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	// 연결할 server의 socket address 설정
	bzero((char *)&servAddr, sizeof(servAddr));	// 0으로 초기화
	servAddr.sin_family = PF_INET;	// IPv4
	// port 번호를 network by order 2byte integer로 변환하여 지정
	servAddr.sin_port = htons(SERV_TCP_PORT);

	// IP 주소 지정
	if (isdigit(argv[1][0]))  {
		// 받아온 argument의 첫 번째 값이 숫자인 경우
		// -> dotted decimal ip 주소로 받은 경우 
		// -> inet_addr를 사용해 network by order 4byte integer로 변환
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else  {
		// 첫 번째 글자가 숫자가 아닌 경우
		// -> domain name으로 받은 경우 
		// -> gethostbyname을 사용해 network by order 4byte integer로 변환
		if ((hp = gethostbyname(argv[1])) == NULL)  {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}

	// tcp server와 connection 맺기
	if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)  {
		perror("connect");
		exit(1);
	}

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
