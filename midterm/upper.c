#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	FILE	*src, *dst;
	char	ch;
	int		nchar = 0;
	int		nupchar = 0;
	int		nspace = 0;
	int		nnum = 0;
	int		nspecial = 0;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	if ((dst = fopen(argv[2], "wt")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	while ((ch = fgetc(src)) != EOF)  {
		nchar += 1;
		if (ch >= 97 && ch <= 122) {
			ch = ch - 32;
			nupchar += 1;
		}
		else if (ch >= 65 && ch <= 90) continue;
		else if (ch == 32) { // 공백
			nspace += 1;
		}
		else if (ch >= 48 && ch <= 57 ) { //숫자인 경우
			nnum += 1;
		}
		else { // 문자와 숫자가 아닌 경우
			nspecial += 1;
		}
		fputc(ch, dst);
	}

	printf("Converted file: %s\n", argv[2]);
	printf("Total characters: %d\n", nchar);
	printf("Converted characters: %d\n", nupchar);
	printf("Blank characters: %d\n", nspace);
	printf("Number characters: %d\n", nnum);
	printf("Special characters: %d\n", nspecial);

	fclose(src);
	fclose(dst);
}
