#include <stdio.h>
#include <stdlib.h>

/* upper.c
 * text file을 입력받아 모두 대문자로 변경하여 출력하는 프로그램
 * 실행파일을 포함하여 program argument는 2개여야한다(실행파일, 변환할 파일)
 * $ ./upper upper.c
 */
int main(int argc, char* argv[])
{
	FILE 	*fp;
	char	ch;

	// argument가 2개가 아닌 경우 프로그램 종료
	if (argc != 2) {
		fprintf(stderr, "Usage: %/s file\n", argv[0]);
		exit(1);
	}

	// argv[1]에 해당하는 입력받은 file을 text file read option으로 open
	if ((fp = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}

	printf("-----Upper-----\n");

	// loop를 돌며 문자 하나씩 받아와 알파벳 소문자인지 검사
	// 알파벳 소문자(ASCII 97~122)라면 대문자(ASCII 65~90)로 변환후 출력
	// 알파벳 소문자가 아니라면 그대로 출력
	// 더 이상 읽어올 내용이 없으면(=EOF) loop 종료
	while ((ch = fgetc(fp)) != EOF) {
		if(ch >= 97 && ch <= 122) {
			putchar(ch - 32);
		}
		else {
			putchar(ch);
		}
	}
	
	// 파일 사용이 끝났으므로 파일을 close
	fclose(fp);
}
