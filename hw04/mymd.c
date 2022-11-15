#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

/* mymd.c
 * 새로운 directory를 생성하는 프로그램
 * argument는 총 2개 필요. 생성할 directory name
 * $ gcc -o mymd mymd.c (or make mymd)
 * $ ./mymd test
 * $ ls -l | grep test
 */
main(int argc, char *argv[])
{
	// 받아온 argument의 수가 2가 아닌 경우 프로그램 종료
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);	// 오류 메세지 출력
		exit(1);
	}

	// 생성할 directory name(argv[1])으로 새로운 directory 생성
	/* 755 permission */
	if (mkdir(argv[1], 0755) < 0)  {	// 755 permission mode로 새로운 directory 생성
		perror("mkdir");	// 오류 메세지 출력
		exit(1);
	}
}
