#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

/* umask.c
 * bar, foo 파일을 생성해 permission mode를 설정하는 프로그램
 * argument 필요하지 않음
 * $ gcc -o umask umask.c (or make umask)
 * $ ./umask
 * $ ls -l bar foo
 * -rw-rw-rw-   1   cjs     other   0   Aug
 * -rw-------   1   cjs     other   0   Aug
 */
main()
{
    umask(0);   // mask 설정을 아무것도 하지 않음
    // bar file을 생성하며 owner read/write, group read/write, other read/write 옵션 설정
    if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH) < 0)  {  
        perror("creat");    // 오류 메세지 출력
        exit(1);
    }

    /* Set file mode creation mask: S_IRGRP, S_IWGRP, S_IROTH, S_IWOTH */
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    // foo file을 생성하며 bar file과 동일하게 옵션 설정
    // 하지만 mask 설정으로 인해 owner read/write의 비트만 켤 수 있음
    if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
                S_IROTH | S_IWOTH) < 0)  {
        perror("creat");    // 오류 메세지 출력
        exit(1);
    }
}
