#include <stdio.h>

enum { FALSE, TRUE };

/* tab.c
 * tab을 공백 4개로 변환해주는 프로그램
 * program argument를 정해진 수 없이 여러 개 받을 수 있음 
 * gcc -o tab tab.c (or make tab)
 * ./tab *.c
 */

// 입력받은 file에 대해 tab을 공백 4개로 변환해주는 함수
void
conv(char *fname)
{
	FILE	*src, *dst;	// file type pointer 선언
	char	fdest[40];	// output file의 이름이 저장될 문자열 
	int		ch, first;

	// source file을 text file read option으로 open
	if ((src = fopen(fname, "rt")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		return;
	}

	strcpy(fdest, fname);	// source file의 filename을 fdest 문자열에 복사
	strcat(fdest, ".t");	// fdest 문자열의 끝부분에 ".t" 추가 삽입
	// destination file을 text file write option으로 open
	if ((dst = fopen(fdest, "wt")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		return;
	}


	// line의 첫 부분에만 tab이 존재할 수 있으므로 line의 첫 부분인지 여부 저장
	first = TRUE;
	// loop를 돌며 문자 하나씩 copy
	// 더 이상 읽어올 내용이 없으면 loop 종료
	while ((ch = fgetc(src)) != EOF)  {
		// line의 첫부분이고, 읽어온 문자가 tab일 경우 tab 대신 공백 4개를 삽입 
		if (first && ch == '\t')  {
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
		}
		// line의 첫부분이 아니면(읽어온 문자가 tab이 아닌 경우) 문자 그대로 삽입
		else  {
			fputc(ch, dst);
			if (first) // line의 첫부분은 이미 읽었으므로 first의 값을 FLASE로 변경
				first = FALSE;
			if (ch == '\n') // 줄바꿈을 만나면 first의 값을 TRUE로 바꾸기
				first = TRUE;
		}
	}

	// 파일 사용이 끝났으므로 파일을 close
	fclose(src);
	fclose(dst);
}

// main 함수와 tab 변환 함수를 분리
main(int argc, char *argv[])
{
	// loop를 돌며 argument로 입력받은 모든 파일에 대해 conv 함수 수행
	while (--argc)  {
		conv(argv[argc]);
	}
}

