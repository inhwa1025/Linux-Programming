#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

/* chmod.c
 * permission mode를 변경하는 프로그램
 * argument 필요하지 않음
 * $ gcc -o chmod chmod.c (or make chmod)
 * $ ./chmod
 * $ ls -l bar foo
 * -rwSr--rw-   1   cjs     other   0   Aug
 * -rw-r--r--   1   cjs     other   0   Aug
 */
main()
{
    struct stat statbuf;    // stat 구조체 선언

    // bar file에 대한 state 정보를 받아옴
    if (stat("bar", &statbuf) < 0)  {
        perror("stat");     // 오류 메세지 출력
        exit(1);
    }

    // bar file의 permission mode를 변경
    // & ~S_IWGRP => S_IWGRP의 bit를 끄기
    // S_ISUID 설정
    if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0)  {
        perror("chmod");    // 오류 메세지 출력
        exit(1);
    }

    /* S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH : ON */
    if (chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)  {
        perror("chmod");    // 오류 메세지 출력
        exit(1);
    }
}
