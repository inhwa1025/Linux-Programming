#include <stdio.h>

/* tcp1.c
 * text를 복사해주는 프로그램
 * 한 번에 character 하나씩 복사하도록 작성 됨
 * 해당 프로그램 실행 시 argument는 총 3개 필요
 * $ gcc -o tcp1 tcp1.c (or make tcp1)
 * $ ./tcp1 tcp1.c tcp1.bak
 * $ ls -l tcp1.c tcp1.bak
 */
int
main(int argc, char *argv[])
{
	FILE	*src, *dst;	// file type pointer 선언
	int		ch;	// 하나씩 복사 할 character를 저장할 변수

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

	// loop를 돌며 src 파일에서 character 하나 씩 읽어와 dst 파일에 쓰기
	// 더 이상 읽어올 내용이 없을 때 fgetc 함수는 EOF를 return하므로 loop 종료 
	while ((ch = fgetc(src)) != EOF)  {
		fputc(ch, dst);
	}

	// 파일 사용이 끝났으므로 파일을 close
	fclose(src);
	fclose(dst);
}
