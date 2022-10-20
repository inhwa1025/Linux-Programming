#include <stdio.h>

main()
{
	char c, s[80];
	int i; long l;
	float f; double d;
	
	// c에서는 reference type이 없어 pointer를 사용하여 입력받음
	// 문자열은 자체가 pointer이기 때문에 &s로 쓰지 않고 s로 쓴다.
	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d); 
	printf("Output: %c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d); 

	c = getchar();	// 문자 하나 입력받기
	putchar(c);	// 문자 하나 출력

	gets(s);	// 문자열 입력받기
	puts(s);	// 문자열 출력
}
