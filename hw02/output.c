// C언어는 procedual(절차적)언어이므로 function library를 사용한다
#include <stdio.h>

main()
{
	char c = 'a', s[] = "hello";	// C언어에서 사실상 string의 개념은 없고 s[]는 char array이다
	int i = 100; long l = 99999;
	float f = 3.14; double d = 99.999;
	int *p = &i;	// 포인터 선언
	
	// format string: 문자 하나, 문자열, 10진수, 16진수, long 10진수, float 소수점 넷째짜리까지, long float 소수점 둘째짜리까지, 포인터
	printf("Output: %c %s %d %#X %ld %.4f %.2lf %p\n", c, s, i, i, l, f, d, p); 
	putchar(c);	// 문자 하나 출력
	puts(s);	// 문자열 출력
}
