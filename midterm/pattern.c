#include <stdio.h>
#include <stdlib.h>
#define	MAX_BUF	1024

int
main(int argc, char *argv[])
{
	FILE	*fp;
	int 	count = 0;
	char	buf[MAX_BUF];
	char	*find = argv[2];
	int		size = sizeof(argv[2]);

	if ((fp = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	while (fread(buf, MAX_BUF, 1, fp) > 0)  {
		for (int i = 0; i < MAX_BUF-size; i++) {
			char tmp[size];
			for (int j = 0; j < size ; j++) {
				tmp[j] = buf[i+j];
			}
			if (tmp == find) {
				count += 1;
			}
		}
	}
	printf("%d\n", count);
	fclose(fp);
}
