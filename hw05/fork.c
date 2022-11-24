#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/* fork.c
 * child process를 생성하고 전역변수를 증가시켜 parent process와 child process의 결과를 비교하는 프로그램
 * $ gcc -o fork fork.c (or make fork)
 * $ ./fork
 */

int     Var = 6;    // 전역변수 선언
char    Buf[] = "a write to stdout\n";  // 출력할 문자열

main()
{
    int     var;    // 지역변수 선언
    pid_t   pid;    // 생성한 child process의 process id를 저장할 변수

    var = 88;
    write(STDOUT_FILENO, Buf, sizeof(Buf)-1);   // printf(Buf)와 동일함
    printf("Before fork\n");

    if ((pid = fork()) < 0)  {  // child process를 생성하고 process id를 pid에 저장
        perror("fork");     // 오류 메세지 출력
        exit(1);
    }
    else if (pid == 0)  {   // child process
        /* child */
        Var++;  // 전역 변수 증가 시키기
        var++;  // 지역 변수 증가 시키기
    }
    else  {     // parent process
        /* parent */
        sleep(2);   // 2초동안 sleep
    }

    // child와 parent에 의해 두 번 print됨. Scheduling algorithm에 따라 먼저 실행되는 것은 그때그때 다름
    // parent는 2초 sleep되었다 실행되므로 child가 먼저 출력됨
    printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);
    // 출력 결과:
    //  pid = 18164, Var = 7, var = 89
    //  pid = 18163, Var = 6, var = 88
}
