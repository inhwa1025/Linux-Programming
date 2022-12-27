/* order.c
 * Intel CPU의 경우: Byte Order little-endian
 * 그 외 대다수: Byte Order big-endian
 */
#include <stdio.h>

main()
{
	int		a = 0x12345678;
	char	*p = (char *)&a;

#if 1
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
#else
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
#endif
}
