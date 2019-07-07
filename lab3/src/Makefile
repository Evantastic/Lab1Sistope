# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

CC=gcc

FLAGS = -g -Wall -lpthread -lm

all: lab3

lab3: lab3.o aux.o
	gcc lab3.o aux.o -o lab3 ${FLAGS}

lab3.o: lab3.c header.h
	gcc lab3.c -c ${FLAGS}

aux.o: aux.c header.h
	gcc aux.c -c ${FLAGS}

clean:
	rm lab3 *.o -f
