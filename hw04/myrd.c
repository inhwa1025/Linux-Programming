#include <stdio.h>
#include <unistd.h>

/* myrd.c
 * 빈 directory를 삭제하는 프로그램
 * argument는 총 2개 필요. 삭제할 directory name
 * $ gcc -o myrd myrd.c (or make myrd)
 * $ ./myrd test
 * $ ls -l | grep test
 * $ ./myrd abc1234
 */
main(int argc, char *argv[])
{
	// 받아온 argument의 수가 2가 아닌 경우 프로그램 종료
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);	// 오류 메세지 출력
		exit(1);
	}

	// 입력받은 directory(argv[1])을 삭제
	if (rmdir(argv[1]) < 0)  {
		perror("rmdir");	// 오류 메세지 출력
		exit(1);
	}
}
