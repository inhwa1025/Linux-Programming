# 2022-2 KHU Linux System Programming

2022-2 KHU Linux System Programming

## HW01 - Development Tools in Linux

- Makefile : Makefile의 예시와 각 line에 대한 설명
- hello.c : string copy와 출력 예제 프로그램
- mystrcpy.c : while 반복문에 대한 예제 프로그램
- bug.c : strcpy : for 반복문에 대한 예제 프로그램

## HW02 - Quick Introduction to C

- Makefile
- hello.c : 문자열 출력 예제 프로그램
- output.c : 각각의 자료형 출력에 대한 예제 프로그램
- input.c : 각각의 자료형 입력에 대한 예제 프로그램
- args.c : main 함수의 파라미터로 주어지는 argument 갯수, argument 리스트에 대한 예제 프로그램
- string.c : string 함수들(strlen, strcpy, strcat, strcmp)을 직접 구현한 프로그램

## HW03 - File I/O

- Makefile
- mycp.c : source file의 내용을 destination file로 copy하는 프로그램
- hole.c : 1024 크기의 dummy file을 생성하는 프로그램
- list.c : text file의 내용을 line number와 함께 출력해주는 프로그램
- tcp1.c : text를 character 하나씩 copy하는 프로그램
- tcp2.c : source text file의 내용을 destination text file로 한 줄씩 copy하는 프로그램
- mycp2.c : binary I/O Stream을 이용해 구현한 file copy 프로그램
- split.c : 하나의 source file을 두 개의 destination file로 나누어서 저장하는 프로그램
- merge.c : 두 개의 source file을 합쳐 하나의 destination file로 저장하는 프로그램
- test.in : test input record file
- record.h : Record struct 선언
- conv.c : test.in text file을 Record 단위로 읽어와 binary file로 변환하는 프로그램
- access.c : binary file을 읽어와 순서대로 출력, 임의의 순서로 출력, 내용 수정 후 출력하는 프로그램
- tab.c : file 안에 존재하는 모든 tab을 공백 4개로 변환해주는 프로그램
- upper.c : text file을 입력받아 모두 대문자로 변경하여 출력하는 프로그램

## HW04 - Files & Directories

- Makefile
- stat.c : file의 i-node 정보를 출력시켜주는 프로그램
- umask.c : bar, foo 파일을 생성해 permission mode를 설정하는 프로그램
- chmod.c : bar, foo 파일의 permission mode를 변경하는 프로그램
- myln.c : hard link를 생성하는 프로그램
- mysln.c : symbolic link를 생성하는 프로그램
- myrm.c : file 또는 directory를 삭제하는 프로그램
- mymv.c : file 또는 directory의 이름을 바꾸는 프로그램
- mymd.c : 새로운 directory를 생성하는 프로그램
- myrd.c : 빈 directory를 삭제하는 프로그램
- myls.c : 현재 디렉토리에 있는 파일 목록들을 한 줄에 하나씩 출력하는 프로그램
- mylsr.c : 현재 디렉토리의 파일 목록과 하위 디렉토리의 파일 목록도 recursive하게 출력하는 프로그램
- mycd.c : working directory를 변경하는 프로그램
- mypwd.c : 현재 working directory를 출력하는 프로그램
- mylsl.c : ls -l와 동일하게 동작하는 프로그램
    - 첫 줄에 total blocks를 출력, 이후 각 file의 permission mode, nlink, user name, group name, file size, ctime, filename 출력

## HW05 - Processes & Threads

- Makefile
- fork1.c : child process를 생성하고 process id를 출력하는 프로그램
- fork.c : child process를 생성하고 전역변수를 증가시켜 parent process와 child process의 결과를 비교하는 프로그램
- exit.c : exit handler를 등록하는 프로그램
- wait.c : child process가 종료되길 기다렸다 child의 종료 status를 출력하는 프로그램
- race.c : process의 synchronous한 특성을 이용해 race condition을 발생시키는 프로그램
- worace.c : process의 synchronous한 특성으로 인한 race condition이 발생하지 않도록 wait system call을 사용하여 작성된 프로그램
- env.c : enviroment variable에 접근하여 그 값을 출력하는 프로그램
- exec.c : 프로그램 안에서 다른 프로그램을 실행하는 exec system call을 사용하는 프로그램
- system.c : system() system call을 사용하는 예제 프로그램
- thread.c : thread를 생성하고, thread의 종료를 기다리고, thread를 종료하는 프로그램. 서로 다른 thread들에서 consol에 메세지를 출력하고 종료
- cmd_i.c : iterator를 사용하여 한 process안에서 command들을 순차적으로 수행하는 command-line 프로그램
- cmd_p.c : child process를 생성하여 command들을 처리하는 command-line 프로그램
- cmd_t.c : thread를 생성하여 command들을 처리하는 command-line 프로그램
- mysystem.c : system system call을 구현한 프로그램
- tadd.c : 1부터 100까지의 합을 구하는 프로그램
    - 두 개의 thread를 생성하여 각각 1~50, 51~100까지의 합을 구하고 main thread가 각 thread가 종료되기를 기다린 후 각각의 합을 더하여 출력

## HW06 - Signals

- Makefile
- sig1.c : SIGINT 시그널의 signal handler로 SigIntHandler를 등록하는 프로그램
- sig2.c : SIGUSR1, SIGUSR2 시그널의 signal handler로 SigUsrHandler를 등록하는 프로그램
- mysleep.c : sleep systemcall을 구현한 프로그램
    - SIGALRM signal handler를 등록하고 nsecs 후에 자신에게 SIGALRM 시그널을 보냄
- alarm.c : 1초마다 alarm을 반복하여 signal handler로 등록된 일을 주기적으로 수행하도록 한 프로그램
- nonreent.c : 1초마다 handler가 수행되는 프로그램
    - handler로 등록한 함수에 non-reentrant function을 사용하여 syncronization problem이 발생하게 됨
- sig_thread.c : Thread를 지정하여 signal handler를 등록하는 프로그램
    - 어떤 thread를 지정해도 signal handler는 main thread에서 수행되며 이를 통해 리눅스에서는 시그널을 무조건 main thread에서 처리한다는 사실을 알 수 있음
- cancel.c : pthread_cancel system call을 사용하여 thread를 kill하는 프로그램
- sigwait.c : child process가 종료되길 기다렸다 child의 종료 status를 출력하는 프로그램

## HW07 - **Inter Process Communication**

- Makefile
- pipe.c : pipe system call을 이용하여 parent process에서 child process에게 데이터를 전달하는 프로그램
- synclib.h : synchronization library를 pipe로 구현한 함수들의 헤더 파일
- synclib.c : synchronization library를 pipe로 구현한 프로그램
- sync.c : 미리 구현해둔 synclib.c을 사용해 parent process와 child process가 5번 번갈아가면서 수행되도록 구현한 프로그램
- mymore.c : command에서 'ls -l | more'을 수행한 것과 똑같이 수행되도록 pipe와 dup system call을 사용해 구현한 프로그램
- fifo.h : client-server 프로그램에서 FIFO를 사용하기 위해 기본 설정 변수를 정의한 헤더 파일
- fifos.c : client와 server가 communication을 하기 위해 FIFO를 사용한 server 프로그램
- fifoc.c : client와 server가 communication을 하기 위해 FIFO를 사용한 client 프로그램
- shm.c : shared memory 생성, attach, detach, 삭제하는 예제 프로그램
- shm.h :shared memory 사용을 위해 기본 설정 변수를 정의한 헤더 파일
- sipc1.c : 두 개의 process가 shared memory를 사용하여 통신하는 예제 프로그램
- sipc2.c : 두 개의 process가 shared memory를 사용하여 통신하는 예제 프로그램
- mycp3.c : memory-mapped file을 사용하여 my own cp를 구현한 프로그램
- mipc.c : shared memory에 parent process와 child process가 번갈아가며 변수를 1씩 증가시키는 프로그램
- pipe2.c : 기존 pipe.c 프로그램에서 child도 parent에게 메세지를 전달하도록 추가한 프로그램
- mipcshm.c : 기존의 mipc.c 프로그램을 memory-mapped file 대신 shmget, shmat을 사용하여 구현한 프로그램

## HW08 - Synchronization

- Makefile
- prodcons.h : Producer & Consumer 예제에서 사용될 기본 설정 변수를 정의한 헤더 파일
- prodcons_t.c : Producer & Consumer 예제를 multi-thread 버전으로 구현한 프로그램
    - synchronization을 수행하지 않으므로 critical section problem이 발생할 수 있으며 busy waiting으로 인한 오버헤드 또한 발생할 수 있음
- producer.c : Producer & Consumer 예제를 multi-process 버전으로 구현한 producer 프로그램
    - synchronization을 수행하지 않으므로 critical section problem이 발생할 수 있으며 busy waiting으로 인한 오버헤드 또한 발생할 수 있음
- consumer.c : Producer & Consumer 예제를 multi-process 버전으로 구현한 consumer 프로그램
    - synchronization을 수행하지 않으므로 critical section problem이 발생할 수 있으며 busy waiting으로 인한 오버헤드 또한 발생할 수 있음
- semlib.h : System V semaphore를 사용하여 POSIX semaphore처럼 구현한 함수들의 헤더 파일
- semlib.c : System V semaphore를 사용하여 POSIX semaphore처럼 구현한 프로그램
- producer_s.c : Producer & Consumer 예제를 미리 구현한 semlib.c을 사용하여 multi-process 버전으로 구현한 프로그램
- consumer_s.c : Producer & Consumer 예제를 미리 구현한 semlib.c을 사용하여 multi-process 버전으로 구현한 프로그램
- prodcons.c : Producer & Consumer 예제를 multi-thread 버전으로 구현한 프로그램
    - POSIX semaphore를 사용하여 synchronization을 수행
- dining.c : 밥먹는 철학자 예시에서 젓가락에 semaphore를 설정해 구현한 dining 프로그램
    - 젓가락마다 한 명만 접근하도록 만들 순 있으나 모든 철학자가 동시에 한 쪽 젓가락만 들고 다른 젓가락을 (서로를) 무한히 기다리는 Hold & Wait 상태 발생 가능 → deadlock 필요조건이 모두 만족되므로 deadlock 발생 가능성 있음
- dining2.c : 밥먹는 철학자 예시에서 젓가락이 아닌 철학자와 critical section에 대해 semaphore를 설정하여 구현한 dining2 프로그램
    - Hold & Wait의 가능성을 제거하였으므로 Deadlock은 발생하지 않지만 starvation 발생의 가능성이 있음
- prodcons_m.c : Producer & Comsumer 예제를 Mutex lock과 condition variable을 사용하여 synchronization을 구현한 프로그램
- semlib2.h : mutex와 condition variable을 사용하여 POSIX semaphore를 구현한 함수들의 헤더 파일
- semlib2.c : mutex와 condition variable을 사용하여 POSIX semaphore를 구현한 프로그램
- prodcons_s.c : Producer & Consumer 예제를 thread와 앞서 구현한 semlib2를 이용해 구현한 프로그램
- sipc.h
- sipc_client_sem.c : sipc2 예제를 semaphore 사용을 통해 flag와 busy waiting를 제거하여 구현한 client 프로그램
    - 두 개의 process (server, client)가 shared memory로 통신.
- sipc_server_sem.c : sipc2 예제를 semaphore 사용을 통해 flag와 busy waiting를 제거하여 구현한 server 프로그램
    - 두 개의 process (server, client)가 shared memory로 통신.
- sipc_mutex_t.c : sipc1, sipc2 예제를 multi-thread 버전으로 구현한 프로그램
    - mutex & condition variable을 이용하여 synchronization 수행, shared memory가 아닌 전역변수를 사용하여 통신
- sipc_sem_t.c : sipc1, sipc2 예제를 multi-thread 버전으로 구현한 프로그램
    - semaphore를 이용하여 synchronization 수행, shared memory가 아닌 전역변수를 사용하여 통신

## HW09 - Sockets

- Makefile
- order.c : Integer Byte Ordering을 확인하는 프로그램
    - Intel CPU의 경우: Byte Order little-endian
    - 그 외 대다수: Byte Order big-endian
- tcp.h : tcp 연결을 사용하는 프로그램에서 사용할 변수들을 정의한 헤더 파일
- tcps.c : tcp를 사용하여 프로세스간 통신을 구현한 서버 프로그램
- tcpc.c : tcp를 사용하여 프로세스간 통신을 구현한 클라이언트 프로그램
- udp.h : udp 연결을 사용하는 프로그램에서 사용할 변수들을 정의한 헤더 파일
- udps.c : udp를 사용하여 프로세스간 통신을 구현한 서버 프로그램
- udpc.c : udp를 사용하여 프로세스간 통신을 구현한 클라이언트 프로그램
- tcpc_dns.c : tcp를 사용하여 프로세스간 통신을 구현한 클라이언트 프로그램
    - server의 ip주소뿐만 아니라 hostname을 이용해 connection 맺기 가능
- myusleep.c : select system call을 사용해 usleep을 구현한 프로그램
- select.h : select 프로그램과 select_thread 프로그램에서 사용할 변수들을 정의한 헤더 파일
- select.c : select system call과 multi-process를 사용하여 Synchronous I/O multiplexing을 수행하는 서버 프로그램
- tcps_process.c : 기존 tcps 프로그램을 connection이 맺어진 후 child process를 생성하여 처리하도록 변경한 프로그램
- select_thread.c : select system call과 multi-thread를 사용하여 Synchronous I/O multiplexing을 수행하는 프로그램

## Project

### Putting together into chatting server & client

- Makefile
- chat.h
- chatc.c : multi-threads로 처리
- chats.c : select system call로 multiplexing 처리
