#include <stdio.h>

// char type pointer인 str을 입력받아, str이 가리키는 char 배열 메모리 안에서
// '\0'을 만나기 전까지 loop를 돌며 카운트하여 문자의 개수(문자열 길이) 반환
int
strlen_p(char *str)
{
	int	len = 0;	// return될 길이가 저장될 변수

	while (*str++)  {	// loop를 돌며 문자의 개수 세기
		len++;
	}			// '/0'을 만나면 loop 종료
	return len;		// 총 문자의 개수('\0' 제외) 반환
}

// char array인 str을 입력받아, char array 메모리 안에서
// '\0'을 만나기 전까지 loop을 돌며 카운트하여 문자의 개수(문자열 길이) 반환
int
strlen_a(char str[])
{
	int i;		// loop iterator이자 문자의 개수를 카운트할 변수

	for (i = 0 ; str[i] != '\0' ; i++)
		;	// '\0'을 만나면 loop 종료
	return i;	// 총 문자의 개수('\0' 제외) 반환
}

// char type pointer인 dst와 src를 입력받아, src의 내용을 dst에 복사
void
strcpy_p(char *dst, char *src)
{
	while (*src)  {		// '\0'을 만나기 전까지 loop를 돌며 src의 문자를 dst에 복사
		*dst++ = *src++;	// '\0'을 만나면 loop 종료
	}
	*dst = *src;		// 문자열의 마지막인 '\0'도 복사
}

// char array인 dst와 src를 입력받아, src의 내용을 dst에 복사
void
strcpy_a(char dst[], char src[])
{
	int i;		// loop iterator

	for (i = 0 ; src[i] != '\0' ; i++)	// loop을 돌며 src의 문자를 dst에 복사
		dst[i] = src[i];		// '\0'을 만나면 loop 종료
	dst[i] = src[i];			// 문자열의 마지막인 '\0'도 복사
}

// char type pointer인 dst와 src를 입력받아, src의 내용을 dst의 뒤에 이어붙이기
void
strcat_p(char *dst, char *src)
{
	while (*dst++)	// dst의 가장 마지막 부분으로 pointer 이동
		;
	dst--;		// '/0'의 앞부분으로 pointer 한 칸 이동
	while (*src)  {	// loop를 돌며 src의 문자를 dst의 뒤에 이어붙이기
		*dst++ = *src++;
	}		// '/0'을 만나면 loop 종료
	*dst = *src;	// 문자열의 마지막인 '/0'도 이어붙이기
}

// char array인 dst와 src를 입력받아, src의 내용을 dst의 뒤에 이어붙이기
void
strcat_a(char dst[], char src[])
{
	int i, j;	// loop iterator

	for (i = 0 ; dst[i] != '\0' ; i++)	// dst의 마지막 문자의 위치로 인덱스 이동
		;
	for (j = 0 ; src[j] != '\0' ; j++)	// loop를 돌며 src의 문자를 dst의 뒤에 이어붙이기
		dst[i+j] = src[j];		// '\0'을 만나면 loop 종료
	dst[i+j] = src[j];			// 문자열의 마지막인 '\0'도 이어붙이기
}

// 숙제2 - 추가한 함수
// char type pointer인 dst와 src를 입력받아, dst와 src 두 문자열을 비교
// return: dst>src인 경우 positive integer, dst<src인 경우 negative integer, dst==src인 경우 0
int
strcmp_p(char *dst, char *src)
{
	int ret = 0;	// 문자열의 차이값을 저장할 변수

	while (*dst && *src) {	// dst와 src의 문자를 하나씩 비교
		ret = *dst++ - *src++;
		if (ret != 0)	// 문자가 다른 경우 차이값을 return하고 종료
			return ret;
	}	// dst 또는 src가 '\0'을 만나면 loop 종료

	ret = *dst - *src;	// 두 문자열이 같거나 둘 중 하나가 '\0'을 만난 경우 마지막 문자열 비교
	return ret;
}

// char array인 dst와 src를 입력받아, dst와 src 두 문자열을 비교
// return: dst>src인 경우 positive integer, dst<src인 경우 negative integer, dst==src인 경우 0
int
strcmp_a(char dst[], char src[])
{
	int ret = 0;	// 문자열의 차이값을 저장할 변수
	int i;		// loop iterator
	
	for(i = 0; dst[i] != '\0' && src[i] != '\0'; i++) {	// dst와 src의 문자를 하나씩 비교
		ret = dst[i] - src[i];
		if (ret != 0)		// 문자가 다른 경우 차이값을 return하고 종료
			return ret;
	}		// dst 또는 src가 '\0'을 만나면 loop 종료
	
	ret = dst[i] - src[i];	// 두 문자열이 같거나 둘 중 하나가 '\0'을 만난 경우 마지막 문자열 비교
	return ret;
}

int main()
{
	int		len1, len2;	// strlen의 결과를 저장할 변수
	char	str1[20], str2[20];	// strcpy, strcap의 결과를 저장할 변수

	// strlen() 사용 예시
	len1 = strlen_p("Hello");	// "Hello" 배열의 문자 수를 포인터로 카운트
	len2 = strlen_a("Hello");	// "Hello" 배열의 문자 수를 반복문으로 카운트
	printf("strlen: p=%d, a=%d\n", len1, len2);	// 두 함수의 결과를 출력하여 비교

	// strcpy() 사용 예시
	strcpy_p(str1, "Hello");	// 포인터로 "Hello" 배열을 str1 변수에 복사
	strcpy_a(str2, "Hello");	// 반복문으로 "Hello" 배열을 str2 변수에 복사
	printf("strcpy: p=%s, a=%s\n", str1, str2);	// 두 함수의 결과를 출력하여 비교

	// strcat() 사용 예시
	strcat_p(str1, ", World!");	// 포인터로 ", World!" 배열을 str1 변수에 이어붙이기
	strcat_a(str2, ", World!");	// 반복문으로 ", World!" 배열을 str2 변수에 이어붙이기
	printf("strcat: p=%s, a=%s\n", str1, str2);	// 두 함수의 결과를 출력하여 비교

	// 숙제2 - strcmp() 사용 예시
	int result1, result2;		// strcmp의 결과를 저장할 변수
	// 두 번째 문자열의 길이가 더 긴 경우 -> negative integer return
	result1 = strcmp_p("Hello", "World!");	// 포인터로 문자열 비교
	result2 = strcmp_a("Hello", "World!");	// 반복문으로 문자열 비교
	printf("strcmp(str1<str2):  p=%d, a=%d\n", result1, result2);	// 두 함수의 결과를 출력하여 비교
	// 두 문자열이 같은 경우 -> 0 return
	result1 = strcmp_p("Hello", "Hello");	// 포인터로 문자열 비교
	result2 = strcmp_a("Hello", "Hello");	// 반복문으로 문자열 비교
	printf("strcmp(str1==str2): p=%d, a=%d\n", result1, result2);	// 두 함수의 결과를 출력하여 비교
	// 첫 번째 문자열의 길이가 더 긴 경우 -> positive integer return
	result1 = strcmp_p("Hello, World!", "Hello");	// 포인터로 문자열 비교
	result2 = strcmp_a("Hello, World!", "Hello");	// 반복문으로 문자열 비교
	printf("strcmp(str1>str2):  p=%d, a=%d\n", result1, result2);	// 두 함수의 결과를 출력하여 비교
}
