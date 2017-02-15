CC = g++
CFLAG = -g -O2 -lpthread -Wall

PROGS = server01

all: ${PROGS}

server01: server01.o
	${CC} ${FLAGS} -o $@ server01.o ${LIBS}


