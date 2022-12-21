#define MY_ID 		63
#define SHM_KEY 	(0x9000 + MY_ID)
#define SHM_SIZE    1024
#define SHM_MODE 	(SHM_R | SHM_W | IPC_CREAT)

// client가 request를 보낸 후 +1 하면 server가 wait을 멈추고 
// request를 처리하도록 하기 위한 semaphore. 초기화 값은 0
#define REQUEST_SEM_KEY	(0x5000 + MY_ID)

// server가 reply를 보낸 후 +1 하면 client가 wait를 멈추고 
// reply를 받을 수 있도록 하기 위한 semaphore. 초기값은 0
#define REPLY_SEM_KEY	(0x6000 + MY_ID)

// shared memory(critical section)을 보호하기 위한 semaphore
#define MUTEX_SEM_KEY	(0x7000 + MY_ID)

