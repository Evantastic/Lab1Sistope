CC = gcc
CFLAGS = -g -Wall -lm
BDIR = ./bin
CDIR = ./src

default: lab1 vis

lab1: lab1.o
	$(CC) -o $(BDIR)/lab1 $(BDIR)/lab1.o $(CFLAGS)

lab1.o: $(CDIR)/lab1.c
	$(CC) -c $(CDIR)/lab1.c -o $(BDIR)/lab1.o $(CFLAGS)

vis: vis.o
	$(CC) -o $(BDIR)/vis $(BDIR)/vis.o $(CFLAGS)

vis.o: $(CDIR)/vis.c
	$(CC) -c $(CDIR)/vis.c -o $(BDIR)/vis.o $(CFLAGS)

clean:
	rm $(BDIR)/*
