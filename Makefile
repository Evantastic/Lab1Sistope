CC = gcc
CFLAGS = -g -Wall
BDIR = ./bin
CDIR = ./src

default: lab1 vis

lab1: lab1.o
	$(CC) $(CFLAGS) -o $(BDIR)/lab1 $(BDIR)/lab1.o

lab1.o: $(CDIR)/lab1.c
	$(CC) $(CFLAGS) -c $(CDIR)/lab1.c -o $(BDIR)/lab1.o

vis: vis.o
	$(CC) $(CFLAGS) -o $(BDIR)/vis $(BDIR)/vis.o

vis.o: $(CDIR)/vis.c
	$(CC) $(CFLAGS) -c $(CDIR)/vis.c -o $(BDIR)/vis.o

clean:
	rm $(BDIR)/*
