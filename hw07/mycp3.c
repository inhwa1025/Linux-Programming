/* mycp3.c
 * memory-mapped file을 사용하여 my own cp를 구현한 프로그램
 * $ gcc -o mycp3 mycp3.c (or make mycp3)
 * $ ./mycp3 mycp3.c mycp3.bak
 * $ ls -l mycp3.c mycp3.bak
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

main(int argc, char *argv[])
{
	int			fdin, fdout;	// file descriptor
	char		*src, *dst;		// source file, destination file
	struct stat	statbuf;		// file의 정보를 받아올 statbuf

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	// source file open. read only
	if ((fdin = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");
		exit(1);
	}
	// destination file open. read/write. 0644 permission으로 create
	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}

	// 열려있는 source file의 정보를 받아오기
	if (fstat(fdin, &statbuf) < 0)  {
		perror("fstat");
		exit(1);
	}
	// 빈 파일을 생성했기 때문에 mapping할 게 없으므로
	// copy할 file size만큼의 빈 공간을 생성
	if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0)  {
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1);	// null string (1 byte) write

	// source file을 memory에 mapping -> virtual addess 할당
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	// destination file을 memory에 mapping -> virtual addess 할당
	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}

	// memcpy: 메모리의 값을 복사하는 함수
	// memory-mapped file을 사용하였으므로 memory가 아닌 file에서 copy 발생
	memcpy(dst, src, statbuf.st_size);

	// file 사용이 종료되었으므로 file close
	close(fdin);
	close(fdout);
}
