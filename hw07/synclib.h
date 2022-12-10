#include <sys/types.h>
#include <unistd.h>


void TELL_WAIT(void);	// pipe 생성 함수 -> pipe가 2개 생성 됨 (pipe1, pipe2)
void TELL_PARENT(void);	// pipe2에 데이터를 write하는 함수
void WAIT_PARENT(void);	// pipe1에서 데이터를 read하는 함수
void TELL_CHILD(void);	// pipe1에 데이터를 write하는 함수
void WAIT_CHILD(void);	// pipe2에서 데이터를 read하는 함수
