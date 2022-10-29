#include <stdio.h>

#define	MAX_BUF	1024	// buffer 최대 크기를 1024로 선언

/* merge.c
 * 두 파일을 하나의 파일로 합쳐서 저장하는 프로그램
 * 해당 프로그램 실행 시 argument는 총 4개 필요
 * $ gcc -o merge merge.c (or make merge)
 * $ ./merge a b merge.new
 * $ chmod a+x merge.new
 * $ ./merge.new
 */
int
main(int argc, char *argv[])
{
	FILE	*src1, *src2, *dst;	// file type pointer 선언
	char	buf[MAX_BUF];		// 읽어온 내용을 저장해둘 문자열 선언
	int		count;		// 읽어온 내용의 크기를 저장해둘 변수 선언

	// 입력 받은 argument의 수가 4가 아닌 경우 프로그램 종료
	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source1 source2 dest\n", argv[0]);
		exit(1);
	}

	// merge를 위해 read file은 2개, write file은 1개 open
	// argv[1]의 파일(source1 file)을 binary file read 옵션으로 open
	// 오류 발생(return 값이 NULL인 경우) 프로그램 종료
	if ((src1 = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}
	// argv[2]의 파일(source2 file)을 binary file read 옵션으로 open
        // 오류 발생(return 값이 NULL인 경우) 프로그램 종료
	if ((src2 = fopen(argv[2], "rb")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}
	// argv[3]의 파일(destination file)을 binary file write 옵션으로 open
	// 오류 발생(return 값이 NULL인 경우) 프로그램 종료
	if ((dst = fopen(argv[3], "wb")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}

	// loop를 돌며 src1 file의 내용을 읽어와 dst file에 저장
	// 더 이상 읽어올 내용이 없는 경우 0을 return하므로 loop 종료
	while (count = (fread(buf, 1, MAX_BUF, src1)) > 0)  {
		fwrite(buf, 1, count, dst);
	}

	// loop를 돌며 src2 file의 내용을 읽어와 dst file에 저장
	// 더 이상 읽어올 내용이 없는 경우 0을 return하므로 loop 종료
	while (count = (fread(buf, 1, MAX_BUF, src2)) > 0)  {
		fwrite(buf, 1, count, dst);
	}

	// 파일 사용이 끝났으므로 파일을 close
	fclose(src1);
	fclose(src2);
	fclose(dst);
}

