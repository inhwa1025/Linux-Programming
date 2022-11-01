#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024	// buffer 최대 크기를 1024로 선언

/* mycp.c 
 * source file의 내용을 destination file로 copy하는 프로그램
 * 해당 프로그램 실행 시 argument는 총 3개 필요
 * $ gcc -o mycp mycp.c (or make mycp)
 * $ ./mycp mycp.c mycp.bak
 * $ ls -l mycp.c mycp.bak
 */
int main(int argc, char *argv[])
{
	int 	fd1, fd2, count;	// open한 file descriptor와 읽어온 byte 수를 저장할 변수
	char	buf[MAX_BUF];		// 읽어올 최대 버퍼 크기 만큼 공간 할당

	// 받아온 argument의 수가 3이 아닌 경우 프로그램 종료
	if (argc != 3)  {
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	
	// argv[1]의 파일(source file)을 Read only 옵션으로 open
	// 오류 발생(return 값이 음수인 경우) 프로그램 종료
	if ((fd1 = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");		// 오류 메세지 출력
		exit(1);
	}
	
	// argv[2]의 파일(destination file)을 write only, 파일이 없는 경우 생성, 덮어쓰기 옵션으로 open
	// 0644 -> 8진수 644 -> 2진수 110100100 -> rw-r--r--의 권한 옵션 부여
	// (파일 소유자 read/write 가능, 소유자가 포함된 그룹 read만 가능, 그 외 read만 가능)
	// 오류 발생(return 값이 음수인 경우) 프로그램 종료
	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}
	
	// fd1에서 MAX_BUF만큼 읽어와 buf에 저장한 후 읽은 byte 수를 count에 저장
	// 더 이상 읽어올 내용이 없는 경우 count 값은 0이 되므로 loop 종료
	while ((count = read(fd1, buf, MAX_BUF)) > 0)  {
		write(fd2, buf, count);		// 읽어온 크기만큼 그 내용을 fd2에 write
	}

	// 파일 사용이 끝났으므로 두 파일을 close
	close(fd1);
	close(fd2);
}
