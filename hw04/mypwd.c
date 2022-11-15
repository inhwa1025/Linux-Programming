#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	256		// 최대 buffer의 크기를 256으로 define

/* mypwd.c
 * 현재 working directory를 출력하는 프로그램
 * $ gcc -o mypwd mypwd.c (or make mypwd)
 * $ ./mypwd
 * $ pwd
 */
main(int argc, char *argv[])
{
	char	buf[MAX_BUF];	// MAX_BUF크기를 가지는 문자열 buf 선언

	// 현재 working directory를 받아와 buf에 저장
	if (getcwd(buf, MAX_BUF) == NULL)  {
		perror("getcwd");	// 오류 메세지 출력
		exit(1);
	}

	printf("%s\n", buf);	// 현재 working directory 출력
}
