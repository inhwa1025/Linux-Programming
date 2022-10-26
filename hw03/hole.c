#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/* hole.c
 * 1024 크기의 dummy file을 생성하는 프로그램
 * $ gcc -o hole hole.c (or make hole)
 * $ ./hole
 * $ ls -l file.hole
 */
int main()
{
	int 	fd;		// 생성할 파일의 file descriptor가 저장될 변수 
	char	ch = '\0';	// 0 -> null 문자

	// "file.hole"이라는 빈 파일 생성
	// 파일 접근 옵션 0400 -> r-------- -> 파일 소유자만 read 가능, 그 외 모두 불가능
	// 오류 발생 시(return 값이 음수인 경우) 프로그램 종료
	if ((fd = creat("file.hole", 0400)) < 0)  {
		perror("creat");	// 오류 메세지 출력
		exit(1);
	}

	// file의 가장 앞에서부터 1023 byte 만큼 이동
	// 오류 발생 시(return 값이 음수인 경우) 프로그램 종료 
	if (lseek(fd, 1023, SEEK_SET) < 0)  {
		perror("lseek");	// 오류 메세지 출력
		exit(1);
	}

	// 앞의 1023만큼의 공간까지 모두 ch에 있는 null 문자(0)으로 채워짐
	// 최종적으로 0이 1024개 들어있는 의미 없는 dummy file이 생성됨 
	write(fd, &ch, 1);

	// 파일 사용이 끝났으므로 파일을 close
	close(fd);
}
