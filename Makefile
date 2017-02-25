CC = g++
CFLAG = -g -O2 -Wall
LIBS = -lpthread
PROGS = server01 client01

all: ${PROGS}

server01: server01.o
	${CC} ${FLAGS} -o $@ server01.o ${LIBS}

client01: client01.o
	${CC} ${FLAGS} -o $@ client01.o ${LIBS}
