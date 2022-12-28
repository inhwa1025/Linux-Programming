#define	MY_ID			63

#define	SERV_TCP_PORT	(7000 + MY_ID)
#define	SERV_HOST_ADDR	"127.0.0.1"		// loopback. localhost
// 다른 서버와 통신하려면 SERV_HOST_ADDR을 해당 서버의 주소로 변경
// $ nslookup 을 사용하여 해당 서버의 IP 주소 확인 가능

#define	MSG_REQUEST		1
#define	MSG_REPLY		2


typedef struct  {
	int		type;
	char	data[128];
}
	MsgType;
