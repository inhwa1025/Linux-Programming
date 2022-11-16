#include <stdio.h>
#include <unistd.h>

/* mysln.c
 * symbolic link를 생성하는 프로그램
 * argument는 총 3개 필요. actual path, symbolic path
 * $ gcc -o mysln mysln.c (or make mysln)
 * $ ./mysln mysln.c mysln.c.ln
 * $ ls -l mysln.c mysln.c.ln
 * $ vi mysln.c.ln (& update it)
 * $ vi mysln.c (& check the update)
 * $ ./stat mysln.c mysln.c.ln (& check st_nlink)	-> number of link는 1
 * $ rm mysln.c.ln
 * $ ./stat mysln.c (& check st_nlink)				-> number of link는 1
 */
main(int argc, char *argv[])
{
	/* update */

	// 받아온 argument의 수가 3이 아닌 경우 프로그램 종료
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);	// 에러 메세지 출력
		exit(1);
	}

	// actual path(argv[1])에 대해 symbolic link(argv[2])를 생성
	if (symlink(argv[1], argv[2]) < 0)  {
		perror("link");		// 에러 메세지 출력
		exit(1);
	}
}
