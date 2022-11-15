#include <stdio.h>

/* mymv.c
 * file 또는 directory의 이름을 바꾸는 프로그램
 * argument는 총 3개 필요. 기존 이름, 새로운 이름
 * $ gcc -o mymv mymv.c (or make mymv)
 * $ ./mymv mymv.o oops.o
 * $ ls -l mymv.o oops.o
 * $ ./mymv oops.o ../oops.o	-> file이 ../ 으로 이동됨
 * $ ./mymv ../oops.o .			-> 현재 디렉토리 바깥 영역에 대해서는 해당 작업 불가능
 */
main(int argc, char *argv[])
{
	// 입력받은 argument 수가 3이 아닌 경우 프로그램 종료
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);	// 오류 메세지 출력
		exit(1);
	}

	// 기존 파일의 이름(argv[1])을 새로운 이름(argv[2])으로 변경
	if (rename(argv[1], argv[2]) < 0)  {
		perror("rename");	// 오류 메세지 출력
		exit(1);
	}
}
