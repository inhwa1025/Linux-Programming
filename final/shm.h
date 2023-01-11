#define	MY_ID			63
#define	SHM_KEY			(0x9000 + MY_ID)
#define	SHM_MODE		(SHM_R | SHM_W | IPC_CREAT)

#define	ONE_SEM_KEY		(0x5000 + MY_ID)
#define TWO_SEM_KEY		(0x1000 + MY_ID)

#define	MAX_BUF			2

typedef struct  {
	int		data;
}
	ItemType;

typedef struct  {
	ItemType	buf[MAX_BUF];
	int			in;
	int			out;
	int			counter;
}
	BoundedBufferType;

#define	SHM_SIZE	sizeof(BoundedBufferType)
