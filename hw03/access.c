#include <stdio.h>
#include <unistd.h>
#include "record.h"

// record를 한 줄씩 출력하는 함수
void
print_record(Record *rp)
{
	puts(rp->stud);
	puts(rp->num);
	puts(rp->dept);
}

/* access.c
 * binary file을 읽어와 순서대로 출력, 임의의 순서로 출력, 내용 수정 후 출력하는 프로그램
 * $ gcc -o access access.c (or make access)
 * $ ./access test.out
 */
main(int argc, char *argv[])
{
	FILE	*fp;	// file type pointer 선언
	Record	rec;	// record structure 선언

	// 입력 받은 argument의 수가 2가 아닌 경우 프로그램 종료
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}

	// argv[1]의 파일을 binary file read+(write 가능) option으로 open
	if ((fp = fopen(argv[1], "r+b")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	printf("-----Ordered Record List-----\n");
	
	// loop를 돌며 record 단위로 순서대로 내용 출력
	// 더 이상 읽어올 내용이 없으면 EOF(=-1)을 출력하므로 loop 종료
	while (fread(&rec, sizeof(rec), 1, fp) > 0)  {
		print_record(&rec);
	}
	rewind(fp);	// pointer를 file의 첫 부분으로 rewind
	getchar();	// 다음 출력을 잠시 미룸. enter to continue

	printf("-----Shuffled Record List (3, 6, 2, 4, 1, 5)-----\n");

	// 레코드 크기의 배수만큼 포인터를 움직여서 랜덤한 순서로 내용을 출력 
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);	// 임의의 위치에서 레코드 읽어오기
	print_record(&rec);			// 레코드 내용 출력

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*1L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*3L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*0L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*4L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	getchar();	// 다음 출력을 잠시 미룸. enter to continue

	printf("-----Updated Record List-----\n");

	// 3번째 위치한 레코드의 department를 Movie로 수정
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Movie");
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);

	// 5번째 위치한 레코드의 department를 Baseball로 수정
	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Baseball");
	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);

	// file pointer의 위치를 파일의 가장 처음으로 rewind
	rewind(fp);
	// loop를 돌며 file의 모든 record 내용을 출력
	while (fread(&rec, sizeof(rec), 1, fp))  {
		print_record(&rec);
	}

	// 파일 사용이 끝났으므로 파일을 close
	fclose(fp);
}
