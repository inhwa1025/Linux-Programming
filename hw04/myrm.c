#include <stdio.h>

/* myrm.c
 * file 또는 directory를 삭제하는 프로그램
 * 원하는 만큼 argument를 넘길 수 있음
 * $ gcc -o myrm myrm.c (or make myrm)
 * $ ./myrm myrm.o
 * $ ls -l myrm.o
 * $ ./myrm *.o
 * $ ./myrm abc1234
 */
main(int argc, char *argv[])
{
	int		i;	// argument 수 만큼 반복을 위한 index

	// 입력받은 argument 수가 1인 경우(삭제할 파일을 지정하지 않은 경우) 프로그램 종료
	if (argc == 1)  {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);	// 오류 메세지 출력
		exit(1);
	}

	// 입력받은 file 또는 directory의 수(argument의 수)만큼 반복
	for (i = 1 ; i < argc ; i++)  {
		if (remove(argv[i]) < 0)  {		// file 또는 directory 삭제
			perror("remove");			// 오류 메세지 출력
			exit(1);
		}
	}
}
