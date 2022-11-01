#include <stdio.h>

#define	MAX_BUF	1024	// buffer의 최대 크기를 1024로 선언

/* mycp2.c
 * binary I/O Stream을 이용해 구현한 file copy 프로그램
 * 해당 프로그램 실행 시 argument는 총 3개 필요
 * $ gcc -o mycp2 mycp2.c (or make mycp2)
 * $ ./mycp2 mycp2.c mycp2.bak
 * $ ls -l mycp2.c mycp2.bak
 */
int
main(int argc, char *argv[])
{
	FILE	*src, *dst;	// file type pointer 선언 
	char	buf[MAX_BUF];	// 읽어올 최대 버퍼 크기 만큼 공간 할당 
	int		count;	// 읽어온 크기를 저장할 변수 

	// 받아온 argument의 수가 3이 아닌 경우 프로그램 종료
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	// argv[1]의 파일(source file)을 binary file read 옵션으로 open
	// 오류 발생(return 값이 NULL인 경우) 프로그램 종료
	if ((src = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}

	// argv[2]의 파일(destination file)을 binary file write 옵션으로 open
	// 오류 발생(return 값이 NULL인 경우) 프로그램 종료
	if ((dst = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}

	// loop를 돌며 src 파일에서 1 byte씩 읽어와 dst 파일에 쓰기 -> System Call과 유사
	// 더 이상 읽어올 내용이 없는 경우 0을 return하므로 loop 종료
	while (fread(buf, MAX_BUF, 1, src) > 0)  {
		fwrite(buf, 1, count, dst);	// 읽어온 만큼 내용을 dst에 write
	}

	// 파일 사용이 끝났으므로 파일을 close
	fclose(src);
	fclose(dst);
}
