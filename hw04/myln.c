#include <stdio.h>
#include <unistd.h>

/* myln.c
 * hard link를 생성하는 프로그램
 * argument는 총 3개 필요. existingpath, newpath
 * $ gcc -o myln myln.c (or make myln)
 * $ ./myln myln.c myln.c.ln	-> hard link 생성으로 number of link가 2로 증가함
 * $ ls -l myln.c myln.c.ln
 * $ vi myln.c.ln (& update it)
 * $ vi myln.c (& check the update)
 * $ ./stat myln.c myln.c.ln (& check st_nlink)
 * $ rm myln.c.ln	-> number of link가 1로 감소함
 * $ ./stat myln.c (& check st_nlink)
 */
main(int argc, char *argv[])
{
	/* update */

	// 받아온 argument의 수가 3이 아닌 경우 프로그램 종료
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);	// 오류 메세지 출력
		exit(1);
	}

	// existing path(argv[1])에 대해 newpath(argv[2]) 이름으로 hard link 생성
	if (link(argv[1], argv[2]) < 0)  {
		perror("link");		// 오류 메세지 출력
		exit(1);
	}
}
