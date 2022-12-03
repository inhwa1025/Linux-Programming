#define	MAX_FIFO_NAME	32

#define	SERV_FIFO	"./.fifo"	// FIFO Name

typedef struct  {
	char	returnFifo[MAX_FIFO_NAME];	// FIFO Name을 전달하기 위함
	char	data[128];
}
	MsgType;
