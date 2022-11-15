#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define	MAX_PATH	256		// 최대 path 길이 지정. recursion 중단 조건이 됨

/* mylsr.c
 * 현재 디렉토리의 파일 목록 출력. 하위 디렉토리의 파일 목록도 recursive하게 출력
 * $ gcc -o mylsr mylsr.c (or make mylsr)
 * $ ./mylsr
 * $ ls -R
 */
void
JustDoIt(char *path)	// 넘겨받은 path의 하위 file 목록을 출력하는 함수
{
	DIR				*dp;	// DIR type pointer 선언
	struct dirent	*dep;	// dirent struct pointer 선언
	struct stat		statbuf;	// state를 저장할 stat struct 선언
	char			fullpath[MAX_PATH];		// MAX_PATH 크기를 가지는 문자열 선언
	
	// path에 대해 directory를 open하고 반환받은 DIR type pointer를 dp에 저장
	if ((dp = opendir(path)) == NULL)  {
		perror("opendir");		// 오류 메세지 출력
		exit(0);
	}
	
	printf("\n%s:\n", path);	// path 출력
	// readdir은 한 번에 하나의 file entry(dirent struct)을 반환
	// 반환받은 diernt struct를 dep에 저장하고 d_name을 출력
	// d_name이 . 이거나 .. 이라면 file이 아니므로 d_name을 출력하지 않음
	// 더 이상 읽어올 내용이 없으면 null pointer(=0)을 반환하므로 반복문 종료
	while (dep = readdir(dp))  {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		printf("%s\n", dep->d_name);	// file name 출력
	}

	// directory안에 하위 directory가 있는지 검사하기 위해 directory의 처음으로 이동
	rewinddir(dp);
	
	// readdir에게서 반환받은 diernt struct를 dep에 저장
	// d_name이 . 이거나 .. 이라면 하위 directory가 아니므로 continue
	while (dep = readdir(dp))  {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		// fullpath에 하위 directory 혹은 file의 path를 저장
		strcpy(fullpath, path);
		strcat(fullpath, "/");
		strcat(fullpath, dep->d_name);
		// fullpath의 state 정보를 읽어오기
		if (lstat(fullpath, &statbuf) < 0)  {
			perror("lstat");	// 오류 메세지 출력
			exit(1);
		}
		// fullpath가 directory라면 recursion하게 파일 목록을 출력하도록 함수 호출
		if (S_ISDIR(statbuf.st_mode))  {
			JustDoIt(fullpath);
		}
	}
	
	// directory 사용이 끝났으므로 close
	closedir(dp);
}

main()
{
	// 현재 디렉토리(.)에 대해 JustDoIt 함수 실행
	JustDoIt(".");
}
