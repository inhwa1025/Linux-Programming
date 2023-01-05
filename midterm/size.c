#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

void fsize (char *fname)
{
	struct stat		fstatbuf;

	if (lstat(fname, &fstatbuf) < 0) {
		perror("lstat");
		exit(1);
	}
	printf("%s %d\n", fname, (int)fstatbuf.st_size);

	pthread_exit(NULL);
}

int main ()
{
	DIR				*dp;
	struct dirent	*dep;
	struct stat		statbuf;
	pthread_t tids[1000];
	int i = 0;

	if ((dp=opendir(".")) == NULL) {
		perror("opendir");
		exit(0);
	}

	while ((dep = readdir(dp))) {
		char *fname = dep->d_name;

		if (strcmp(".", fname) == 0 || strcmp("..", fname) == 0)
			continue;

		if (pthread_create(&tids[i], NULL, (void *)fsize, (void *)fname) < 0) {
			perror("pthread_create");
			continue;
		}
		
		pthread_join(tids[i-1], NULL);
		i += 1;
	}

}
