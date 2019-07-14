# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

CC=gcc

FLAGS = -g -Wall -lpthread -lm

all: lab3

lab3: lab3.o reader.o
	gcc lab3.o reader.o -o lab3 ${FLAGS}

lab3.o: lab3.c header.h
	gcc lab3.c -c ${FLAGS}

reader.o: reader.c header.h
	gcc reader.c -c ${FLAGS}

clean:
	rm lab3 *.o -f
