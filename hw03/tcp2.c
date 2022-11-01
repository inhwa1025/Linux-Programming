#include <stdio.h>

#define	MAX_BUF	256	// buffer 최대 크기를 256으로 선언 

/* tcp2.c
 * source text file의 내용을 destination text file로 한 줄씩 copy하는 프로그램
 * 해당 프로그램 실행 시 argument는 총 3개 필요
 * $ gcc -o tcp2 tcp2.c (or make tcp2)
 * $ ./tcp2 tcp2.c tcp2.bak
 * $ ls -l tcp2.c tcp2.bak
 */
int
main(int argc, char *argv[])
{
	FILE	*src, *dst;	// file type pointer 선언
	char	buf[MAX_BUF];	// 복사한 한 line을 저장할 문자열 변수

	// 입력 받은 argument의 수가 3이 아닌 경우 프로그램 종료
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	// text를 가져올 source file인 argv[1]의 파일을 text file read 옵션으로 open
	// 오류 발생 시(fopen return 값이 NULL인 경우) 프로그램 종료
	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}

	// text를 붙여넣을 destination file인 argv[2] 파일을 text file write 옵션으로 open
	// 오류 발생 시(fopen의 return 값이 NULL인 경우) 프로그램 종료
	if ((dst = fopen(argv[2], "wt")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}

	// loop를 돌며 src 파일에서 한 line씩 읽어와 dst 파일에 쓰기
	// 더 이상 읽어올 내용이 없는 경우 NULL을 return하므로 loop 종료
	while (fgets(buf, MAX_BUF, src) != NULL)  {
		fputs(buf, dst);	// 읽어온 만큼 내용을 dst에 write
	}

	// 파일 사용이 끝났으므로 두 파일을 close
	fclose(src);
	fclose(dst);
}
