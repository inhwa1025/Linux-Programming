#include <stdio.h>

#define	MAX_BUF	256	// buffer 최대 크기를 1024로 선언

/*list.c
 * text file의 내용을 line number와 함께 출력해주는 프로그램
 * 해당 프로그램 실행 시 argument는 총 2개 필요
 * $ gcc -o list list.c (or make list)
 * $ ./list list.c
 */
int
main(int argc, char *argv[])
{
	FILE	*fp;	// file type pointer 선언 
	char	buf[MAX_BUF];	// 읽어올 최대 버퍼 크기 만큼 공간 할당
	int		line;	// file의 내용과 함께 출력할 line number를 저장할 변수

	// 입력 받은 argument의 수가 2가 아닌 경우 프로그램 종료 
	if (argc != 2)  {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	// argv[1]의 파일(text file)을 text file read 옵션으로 open 
	// 오류 발생 시(fopen의 return 값이 NULL인 경우) 프로그램 종료 
	if ((fp = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}

	// line number는 1부터 시작
	line = 1;
	// fgets는 한 line을 읽어오는 함수. buf에 MAX_BUF만큼 file에서 읽어옴
	// 파일을 모두 읽었으면 null pointer(0)를 반환하므로 loop 종료 
	while (fgets(buf, MAX_BUF, fp))  {
		// 읽어온 line의 내용을 line number와 함께 출력
		// line number 출력 후 ++ 연산이 수행 됨
		printf("%4d: %s", line++, buf);
	}

	// 파일 사용이 끝났으므로 파일을 close
	fclose(fp);
}
