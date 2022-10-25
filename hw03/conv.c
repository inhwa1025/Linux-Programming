#include <stdio.h>
#include <string.h>
#include "record.h"

/* conv.c
 * text file을 binary file로 변환하는 프로그램
 * 변환 단위는 Record
 * $ gcc -o conv conv.c (or make conv)
 * $ ./conv test.in test.out
 * $ ls -l test.out
 */
main(int argc, char *argv[])
{
	FILE	*src, *dst;	// file type pointer 선언
	Record	rec;		// Record structure 선언

	// 입력 받은 argument의 수가 3이 아닌 경우 프로그램 종료
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	}

	// argv[1]의 파일(source file)을 text file read 옵션으로 open
	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}
	// argv[2]의 파일(destination file)을 binary file write 옵션으로 open
	if ((dst = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}

	// loop를 돌며 src file의 내용을 record 단위로 읽어와 dst file에 write
	// fgets 함수를 이용해 src 파일에서 SMAX 만큼의 데이터를 읽어와 rec.stud에 저장
	// 더 이상 읽어올 내용이 없으면 null pointer(=0)을 반환하므로 loop 종료
	while (fgets(rec.stud, SMAX, src))  {
		// strchr은 '\n'이 처음 나타난 곳의 pointer를 return
		// '\n'을 '\0'으로 대치
		*strchr(rec.stud, '\n') = '\0';
		// fgets 함수를 이용해 src 파일에서 NMAX 만큼의 데이터를 읽어와 rec.num에 저장
		fgets(rec.num, NMAX, src);
		// '\n'을 '\0'으로 대치
		*strchr(rec.num, '\n') = '\0';
		// fgets 함수를 이용해 src 파일에서 DMAX 만큼의 데이터를 읽어와 rec.dept에 저장
		fgets(rec.dept, DMAX, src);
		// '\n'을 '\0'으로 대치
		*strchr(rec.dept, '\n') = '\0';
		
		// 오브젝트(record) 크기만큼 하나씩 dst 파일에 write
		fwrite(&rec, sizeof(rec), 1, dst);
	}

	// 파일 사용이 끝났으므로 파일을 close
	fclose(src);
	fclose(dst);
}
