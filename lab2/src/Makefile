# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

CC=gcc

FLAGS = -g -Wall -lpthread -lm

all: lab2

lab2: lab2.o threads.o aux.o
	gcc lab2.o threads.o aux.o -o lab2 ${FLAGS}

lab2.o: lab2.c header.h
	gcc lab2.c -c ${FLAGS}

threads.o: threads.c header.h
	gcc threads.c -c ${FLAGS}

aux.o: aux.c header.h
	gcc aux.c -c ${FLAGS}

clean:
	rm lab2 *.o -f