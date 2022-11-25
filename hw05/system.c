#include <stdio.h>
#include <stdlib.h>

/* system.c
 * system() system call의 실행을 살펴보는 프로그램
 * system() = fork() + exec() + wait()
 * - system 함수가 return하는 exit code는 총 4가지
 * 	1. child process 정상 종료 -> exit(0) * (1<<8) -> return 0
 *	2. 수행할 수 없는 명령어로 인해 exec에서 오류 발생 -> exit(127) * (1<<8) = 127*256 -> return 32512 
 *	3. exit n과 같이 CLI에서 직접 exit code를 지정하는 경우 -> return n * (1<<8)
 *	4. child process 생성 중에 오류 발생 혹은 연결 소실 -> exit(-1) * (1<<8) -> return -256
 * $ gcc -o system system.c (or make system)
 * $ ./system
 */
main()
{
	int		status;		// 프로그램의 종료 status를 받아와 저장할 변수

	// date 프로그램 실행 -> exit(0)으로 정상종료됨 -> status에 0이 저장됨
	if ((status = system("date")) < 0)  {
		perror("system");	// 오류 메세지 출력
		exit(1);
	}
	printf("exit status = %d\n", status);	// status의 값은 0

	// 알 수 없는 명령어이므로 비정상적 종료 
	// -> exit(127) -> return 127 * (1<<8) = 127*256 -> status에 32512이 저장됨
	if ((status = system("nosuchcommand")) < 0)  {
		perror("system");	// 오류 메세지 출력
		exit(1);
	}
	printf("exit status = %d\n", status);	// status의 값은 32512

	/* $ who; exit 44 */
	// 여러 명령어를 ;으로 구분하여 실행 가능
	// CLI exit에 의한 종료(44로 지정됨) -> return exit(44) * (1<<8) = 44 * 256
	// -> status에 11264이 저장됨
	if (status = system("who; exit 44"))  {	// 0이 아닌 값이 반환되었으므로 true
		perror("system");	// system: Success
		exit(1);	// main함수가 exit code 1로 종료됨 -> 이후의 코드들은 실행되지 못함
	}
	printf("exit status = %d\n", status);
}

