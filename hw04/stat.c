#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

/* stat.c
 * argument로 넘긴 file의 i-node 정보를 출력시켜주는 프로그램
 * 원하는 만큼 argument를 넘길 수 있음
 * $ gcc -o stat stat.c (or make stat)
 * $ ./stat stat.c          -> stat.c의 정보 출력
 * $ ./stat .               -> 현재 디렉토리의 정보 출력
 * $ ./stat *               -> 현대 디렉토리의 모든 파일에 대한 정보 출력
 * $ ./stat .* * | more
 *   -> 현재 디렉토리의 모든 파일에 대한 정보를 한 페이지만 출력. 엔터를 누르면 한줄 씩 더 출력
 */
main(int argc, char *argv[])
{
	struct stat	statbuf;	// i-node 정보를 저장할 stat 구조체 선언
	char		*mode;		// file type을 저장할 문자열 포인터
	int			i;			// argumnet 수 만큼 반복을 돌리기 위한 index

	// 입력받은 file의 수(argument 수)만큼 반복
	for (i = 1 ; i < argc ; i++)  {
		printf("%s: ", argv[i]);	// file name 출력
		if (lstat(argv[i], &statbuf) < 0)  {	// lstat system call으로 i-node 정보 읽어오기
			perror("lstat");	// 에러 메세지 출력
			continue;			// 다음 file로 넘어가기
		}
		// file type을 확인하여 mode 변수에 어떤 type인지 저장
		if (S_ISREG(statbuf.st_mode))	// S_ISREG: regular file인지 확인
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode))	// S_DIR: directory file인지 확인
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode))	// S_ISCHR: character special file인지 확인
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode))	// S_ISBLK: block special file인지 확인
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode))	// S_ISFIFO: pip 또는 FIFO인지 확인
			mode = "FIFO";
		else if (S_ISLNK(statbuf.st_mode))	// S_ISLINK: symbolic link인지 확인
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode))	// S_ISSOCK: socket인지 확인
			mode = "socket";
		printf("%s\n", mode);	// file type 출력
		// file type & mode의 정보를 10진수로 출력 
		printf("\tst_mode = %d\n", statbuf.st_mode);	
		// file의 i-node number를 10진수로 출력
		printf("\tst_ino = %d\n", statbuf.st_ino);		
		// file system device number를 10진수로 출력
		printf("\tst_dev = %d\n", statbuf.st_dev);		
		// special file의 device number를 10진수로 출력
		printf("\tst_rdev = %d\n", statbuf.st_rdev);	
		// number of link를 10진수로 출력
		printf("\tst_nlink = %d\n", statbuf.st_nlink);	
		// file owner의 user id를 10진수로 출력
		printf("\tst_uid = %d\n", statbuf.st_uid);		
		// file owner가 속한 group의 id를 10진수로 출력
		printf("\tst_gid = %d\n", statbuf.st_gid);
		// regular file의 file size(byte)를 10진수로 출력 
		printf("\tst_size = %d\n", statbuf.st_size);
		// file에 마지막으로 access한 시간을 10진수로 출력
		printf("\tst_atime = %d\n", statbuf.st_atime);
		// file을 마지막으로 수정한 시간을 10진수로 출력
		printf("\tst_mtime = %d\n", statbuf.st_mtime);	
		// file statue가 마지막으로 바뀐 시간을 10진수>로 출력
		printf("\tst_ctime = %d\n", statbuf.st_ctime);	
		// best I/O block size를 10진수로 출력
		printf("\tst_blksize = %d\n", statbuf.st_blksize);	
		// 512byte block으로 몇개가 있는지 10진수로 출력
		printf("\tst_blocks = %d\n", statbuf.st_blocks);
	}
}
