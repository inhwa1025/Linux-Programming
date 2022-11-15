#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

/* myls.c
 * 현재 디렉토리에 있는 파일 목록들을 한 줄에 하나씩 출력
 * $ gcc -o myls myls.c (or make myls)
 * $ ./myls
 * $ ls
 */
main()
{
	DIR				*dp;	// DIR type pointer 선언
	struct dirent	*dep;	// dirent struct pointer 선언

	// 현재 directory를 open. opendir에게서 DIR type pointer를 반환받아 dp에 저장
	if ((dp = opendir(".")) == NULL)  {
		perror("opendir");		// 오류 메세지 출력
		exit(0);
	}

	// readdir은 한 번에 하나의 file entry(dirent struct)을 반환
	// 반환받은 diernt struct를 dep에 저장하고 diernt 구조체 안의 d_name 정보만 출력
	// 더 이상 읽어올 내용이 없으면 null pointer(=0)을 반환하므로 반복문 종료
	while (dep = readdir(dp))  {
		printf("%s\n", dep->d_name);	// file name 출력
	}
	
	// directory 사용이 끝났으므로 close
	closedir(dp);
}
