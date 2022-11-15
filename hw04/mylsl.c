#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

/* mylsl.c
 * ls -l과 같이 동작하는 프로그램 구현
 * 첫 줄에 total blocks를 출력
 * 이후 각 file의 permission mode, nlink, user name, group name, file size, ctime, filename 출력
 * $ gcc -o mylsl mylsl.c (or make mylsl)
 * $ ./mylsl
 * $ ls -l
 */
int
main()
{
    char* path = ".";           // 현재 directory를 path로 지정
    DIR             *dp;        // DIR type pointer 선언
    struct dirent   *dep;       // dirent struct pointer 선언
    struct stat     statbuf;    // state를 저장할 stat struct 선언
    long total_blocks = 0;      // 전체 block의 수를 저장할 변수
    char            *fname;     // file name을 저장할 문자열 포인터
    // 현재 directory를 open
    if ((dp=opendir(path)) == NULL) {
        perror("opendir");      // 오류 메세지 출력
        exit(0);
    }

    // 전체 block 수를 구하기 위해 반복문을 사용하여 file list 탐색
    // readdir에게서 반환받은 directory entry를 dep에 저장
    // 더 이상 읽어올 정보가 없으면 null pointer(=0)를 반환하므로 반복문이 종료됨
    while (dep = readdir(dp)) {
        // d_name이 . 이거나 .. 이라면 하위 directory나 file가 아니므로 continue
        if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
            continue;

        // 해당 file의 state 정보 읽어와 statbuf에 저장
        if (lstat(dep->d_name, &statbuf) < 0) {
            perror("lstat");    // 오류 메세지 출력
            continue;           // 다음 file로 넘어가기
        }

        // 전체 block 수를 저장하는 변수에 현재 file block 수를 더하기
        total_blocks += statbuf.st_blocks;
    }

    // 전체 block 수 출력. 2로 나누어주어야 ls -l의 결과와 같아짐
    printf("total %ld\n", total_blocks/2);

    // directory pointer를 가장 앞으로 rewind
    rewinddir(dp);

    // 각 file의 state 정보를 출력하기 위해 반복문을 사용하여 file list를 탐색
    // readdir에게서 반환받은 directory entry를 dep에 저장
    // 더 이상 읽어올 정보가 없으면 null pointer(=0)를 반환하므로 반복문이 종료됨
    while (dep = readdir(dp)) {
        // file name을 문자열 변수에 저장
        char *fname = dep->d_name;

        // file name이 . 이거나 .. 이라면 하위 directory나 file가 아니므로 continue
        if (strcmp(".", fname) == 0 || strcmp("..", fname) == 0)
            continue;

        // 해당 file의 state 정보 읽어와 statbuf에 저장
        if (lstat(fname, &statbuf) < 0) {
            perror("lstat");
            continue;
        }

        // file의 mode 정보를 받아와 변수에 저장
        mode_t stmode = statbuf.st_mode;
        // permission mode를 구하여 string으로 저장할 문자열 변수를 선언
        char mode[11] = "----------";
        // mode의 0번째 문자: file type
        if (S_ISREG(stmode))            // regular file
            mode[0] = '-';
        else if (S_ISDIR(stmode))       // directory file
            mode[0] = 'd';
        else if (S_ISCHR(stmode))       // character special file
            mode[0] = 'c';
        else if (S_ISBLK(stmode))       // block special file
            mode[0] = 'b';
        else if (S_ISFIFO(stmode))      // pip or FIFO
            mode[0] = 'p';
        else if (S_ISLNK(stmode))       // symbolic link
            mode[0] = 'l';
        else if (S_ISSOCK(stmode))      // socket
            mode[0] = 's';
        // mode[1:11] =>  owner read/write/excute, group read/write/excute, other read/write/excute
        if ((stmode & S_IRUSR) == S_IRUSR) mode[1] = 'r';
        if ((stmode & S_IWUSR) == S_IWUSR) mode[2] = 'w';
        if ((stmode & S_IXUSR) == S_IXUSR) mode[3] = 'x';
        else if ((stmode & S_ISUID) == S_ISUID) mode[3] = 'S';  // 실행 시 유효 사용자 ID를 설정
        if ((stmode & S_IRGRP) == S_IRGRP) mode[4] = 'r';
        if ((stmode & S_IWGRP) == S_IWGRP) mode[5] = 'w';
        if ((stmode & S_IXGRP) == S_IXGRP) mode[6] = 'x';
        else if ((stmode & S_ISGID) == S_ISGID) mode[6] = 'S';  // 실행 시 유효 그룹 ID를 설정
        if ((stmode & S_IROTH) == S_IROTH) mode[7] = 'r';
        if ((stmode & S_IWOTH) == S_IWOTH) mode[8] = 'w';
        if ((stmode & S_IXOTH) == S_IXOTH) mode[9] = 'x';

        // file의 number of link 정보를 받아와 integer 변수에 저장
        int nlink = (int)statbuf.st_nlink;

        // file owner의 user name 정보를 받아와 문자열 변수에 저장
        struct passwd *pwd = getpwuid(statbuf.st_uid);
        char* uname = pwd->pw_name;

        // file owner의 group name 정보를 받아와 문자열 변수에 저장
        struct group *grp = getgrgid(statbuf.st_gid);
        char* gname = grp->gr_name;

        // file의 size 정보를 받아와 integer 변수에 저장
        int stsize = (int)statbuf.st_size;

        // file의 last status change time을 받아와 문자열 변수에 저장
        char* change_t = ctime(&statbuf.st_ctime);
        char c_time[13];
        // memcpy(*dst,*src,size) 함수는 src부터 size만큼을 dst에 strcpy를 수행
        memcpy(c_time, &change_t[4], 12);
        c_time[12] = '\0';

        // permission mode, nlink, user name, group name, file size, ctime, filename 출력
        // file size 출력 시 오른쪽 정렬을 위해 %5d 사용
        printf("%s %d %s %s %5d %s %s\n", mode, nlink, uname, gname, stsize, c_time, fname);
    }

    // directory 사용이 끝났으므로 close
    closedir(dp);

    return 0;
}
                                
