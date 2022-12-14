#define	MY_ID			63		// 본인 lsp 번호로 수정	// - shared memory 설정 정보
#define	SHM_KEY			(0x9000 + MY_ID)				// - shared memory 설정 정보
#define	SHM_MODE		(SHM_R | SHM_W | IPC_CREAT)		// - shared memeory 설정 정보

#define	EMPTY_SEM_KEY	(0x5000 + MY_ID)	// - semaphore key value
#define	FULL_SEM_KEY	(0x6000 + MY_ID)	// - semaphore key value
#define	MUTEX_SEM_KEY	(0x7000 + MY_ID)	// - semaphore key value

#define	NLOOPS			20

#define	MAX_BUF			2

typedef struct  {
	int		data;
}
	ItemType;

typedef struct  {	// Circular Queue Data Structure
	ItemType	buf[MAX_BUF];
	int			in;
	int			out;
	int			counter;
}
	BoundedBufferType;

#define	SHM_SIZE	sizeof(BoundedBufferType)
