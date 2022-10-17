# 컴파일러 지정
CC = gcc
# object files
OBJS = test_analysis.o simuation.o peri.o misc.o opt.o stb.o vect.o
# library directory 경로 선언
DIR = /home/username/lib
# 컴파일 옵션: 디버깅 가능, DIR에 해당하는 경로를 라이브러리로 위치 추가, 링킹하지 않고 컴파일만 수행(.o 파일 생성)
CFLAGS = -g -I$(DIR) -I. -c
# 링킹 옵션: 디버깅 가능한 형태로 실행파일 생성
LFLAGS = -g

# OBJS파일들을 가지고 LFLAGS 옵션으로 컴파일하여 std.o 파일을 생성
# prerequisites에 해당하는 OBJS 파일들 중 하나라도 없거나 변경사항 존재 시 재컴파일 하도록 dependency check
std: $(OBJS)
	$(CC) $(LFLAGS) -o std $(OBJS)

# analysis.c 파일을 가지고 CFLAGS 옵션으로 컴파일하여 test_analysis.o 파일 생성
test_analysis.o: analysis.c test_analysys_v1.h $(DIR)/misc.h $(DIR)/opt.h $(DIR)/vect.h
	$(CC) $(CFLAGS) analysis.c

# DIR 경로에 있는 simulation.c 파일을 가지고 CFLAGS 옵션으로 컴파일하여 simuation.o 파일 생성
simuation.o: $(DIR)/simulation.c $(DIR)/sim.h
	$(CC) $(CFLAGS) $(DIR)/simulation.c

# DIR 경로에 있는 peri.c 파일을 가지고 CFLAGS 옵션으로 컴파일하여 peri.o 파일 생성
peri.o: $(DIR)/peri.c $(DIR)/peri.h $(DIR)/misc.h $(DIR)/opt.h
	$(CC) $(CFLAGS) $(DIR)/peri.c

# DIR 경로에 있는 opt.h 파일을 가지고 CFLAGS 옵션으로 컴파일하여 misc.o 파일 생성
misc.o: $(DIR)/misc.c $(DIR)/misc.h $(DIR)/opt.h
	$(CC) $(CFLAGS) $(DIR)/opt.h

# stb.c 파일을 가지고 CFLAGS 옵션으로 컴파일하여 stb.o 파일 생성
stb.o: stb.c $(DIR)/misc.h $(DIR)/opt.h stb.h
	$(CC) $(CFLAGS) stb.c

# DIR 경로에 있는 vect.c 파일을 가지고 CFLAGS 옵션으로 컴파일하여 vect.o 파일 생성
vect.o: $(DIR)/vect.c $(DIR)/vect.h $(DIR)/opt.h
	$(CC) $(CFLAGS) $(DIR)/vect.c

# make clean 수행 시 실행 됨: 프로그램 make 시 생성된 임시파일들을 모두 지움. 해당하는 오브젝트 파일들을 모두 삭제
clean:
	rm -rf std $(OBJS) *.o

