#include <stdio.h>

#define	MAX_CMD		256

/* cmd_i.c
 * iterator를 사용하여 한 process안에서 command들을 순차적으로 수행하는 command-line 프로그램
 * $ gcc -o cmd_i cmd_i.c (or make cmd_i)
 * $ ./cmd_i
 */

// 입력받은 command를 수행하는(척하는) 함수
void
DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

main()
{
	char	cmd[MAX_CMD];	// 사용자에게 입력받은 command를 저장할 문자열 변수

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	// 사용자가 입력할 때까지 기다리게 됨
		if (cmd[0] == 'q')	// 사용자가 'q' 입력시 프로그램 종료
			break;

		DoCmd(cmd);
	}

	/* iterator를 사용하여 한 process 내에서 처리될 때의 문제점
	 * 실행 시간이 긴 command의 경우 다른 command 요청 불가
	 * -> process나 thread를 생성하여 command를 처리하도록하여 해결 가능
	 */
}
