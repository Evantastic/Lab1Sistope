# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

CC=gcc

FLAGS = -g -Wall -lm

all: lab3

lab3: lab3.o reader.o command_line.o
	gcc lab3.o reader.o command_line.o -o lab3 ${FLAGS}

lab3.o: lab3.c header.h
	gcc lab3.c -c ${FLAGS}

reader.o: reader.c header.h
	gcc reader.c -c ${FLAGS}

command_line.o: command_line.c header.h
	gcc command_line.c -c ${FLAGS}
clean:
	rm lab3 *.o -f
