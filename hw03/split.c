#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	1024	// buffer 최대 크기를 1024로 선언

// filesize를 구하는 함수
// filesize를 구하고싶은 file의 file type pointer를 파라미터로 입력받음
long
filesize(FILE *fp)
{
	// 현재 파일의 위치를 임시로 저장해둘 변수와 파일의 크기를 저장할 변수 선언
	long	cur, size;

	cur = ftell(fp);	// ftell은 filepointer의 현재 위치를 return해줌. 현재 위치를 임시 저장
	fseek(fp, 0L, SEEK_END);	// file의 끝으로 file pointer 이동
	size = ftell(fp);	// 현재 파일의 위치가 곧 file의 size가 됨
	fseek(fp, cur, SEEK_SET); 	// file pointer를 원래 있던 위치로 되돌리기
	return(size);	// 구한 filesize를 return
}

/* split.c
 * 하나의 파일을 두 개의 파일로 나누어서 저장하는 프로그램
 * 해당 프로그램 실행 시 argument는 총 4개 필요
 * $ gcc -o split split.c (or make split)
 * $ ./split merge a b
 * $ ls -l merge a b
 */
int 
main(int argc, char *argv[])
{
	FILE	*src, *dst1, *dst2;	// file type pointer 선언
	char	buf[MAX_BUF];		// 복사한 내용을 저장할 문자열 변수
	int		count, size;	// 읽어온 내용의 크기와 파일의 사이즈를 저장해둘 변수 선언

	// 입력 받은 argument의 수가 4가 아닌 경우 프로그램 종료
	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
		exit(1);
	}

	// split을 위해 read file은 1개, write file은 2개 open
	// argv[1]의 파일(source file)을 binary file read 옵션으로 open
	// 오류 발생(return 값이 NULL인 경우) 프로그램 종료
	if ((src = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}

	// argv[2]의 파일(destination1 file)을 binary file write 옵션으로 open
	// 오류 발생(return 값이 NULL인 경우) 프로그램 종료
	if ((dst1 = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}
	// argv[3]의 파일(destination2 file)을 binary file write 옵션으로 open
	// 오류 발생(return 값이 NULL인 경우) 프로그램 종료
	if ((dst2 = fopen(argv[3], "wb")) == NULL)  {
		perror("fopen");	// 오류 메세지 출력
		exit(1);
	}

	// source file의 size를 구하여 반으로 나누기 (반씩 split하여 저장할 것이므로)
	size = filesize(src) / 2;

	// loop를 돌며 src file 중 앞에서부터 절반까지 읽어와 dst1 file에 저장
	// 읽어온 크기(count)를 size에 계속 빼주면서, size가 0이되면 (size만큼 다 읽어오면) loop 종료
	while (size > 0)  {
		// 남은 size가 MAX_BUF보다 큰 경우 MAX_BUF만큼 읽어오고, 아니라면 size만큼 읽어오기
		count = (size > MAX_BUF) ?  MAX_BUF : size;
		fread(buf, 1, count, src);	// src에서 count만큼 읽어와 buf에 저장
		fwrite(buf, 1, count, dst1);	// 읽어온 내용인 buf를 dst1에 write
		size -= count;	// 남은 size 구하기
	}

	// loop를 돌며 src file의 나머지 부분을 읽어와 dst2 file에 저장
	// 더 이상 읽어올 내용이 없는 경우 0을 return하므로 loop 종료
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0)  {
		fwrite(buf, 1, count, dst2);
	}

	// 파일 사용이 끝났으므로 파일을 close
	fclose(src);
	fclose(dst1);
	fclose(dst2);
}
