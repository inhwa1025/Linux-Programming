#include <stdio.h>
#include <unistd.h>

/* mycd.c
 * working directory를 변경하는 프로그램
 * argument는 2개 필요. 변경할 directory path name
 * $ gcc -o mycd mycd.c (or make mycd)
 * $ pwd
 * $ ./mycd ..
 * $ pwd		-> 바뀌지 않음
 */
main(int argc, char *argv[])
{
	// 받아온 argument의 수가 2가 아닌 경우 프로그램 종료
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);	// 오류 메세지 출력
		exit(1);
	}

	// 입력받은 pathname(argv[1])으로 working directory 변경
	if (chdir(argv[1]) < 0)  {
		perror("chdir");	// 오류 메세지 출력
		exit(1);
	}
}
