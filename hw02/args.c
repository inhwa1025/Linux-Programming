#include <stdio.h>

// argument 갯수, argument 리스트
main(int argc, char *argv[])
{
	int		i;	// loop iterator

	for (i = 0 ; i < argc ; i++)	// loop를 돌며 입력받은 index 번호와 argument를 하나씩 출력
		printf("argv[%d]=%s\n", i, argv[i]);
}
