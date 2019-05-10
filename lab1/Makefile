CC = gcc
CFLAGS = -g -Wall -lm
BDIR = .
CDIR = ./src

default: lab1 vis

lab1: lab1.o aux.o
	$(CC) -o $(BDIR)/lab1 $(BDIR)/lab1.o $(BDIR)/aux.o $(CFLAGS)

lab1.o: $(CDIR)/lab1.c $(CDIR)/header.h
	$(CC) -c $(CDIR)/lab1.c -o $(BDIR)/lab1.o $(CFLAGS)

aux.o: $(CDIR)/aux.c $(CDIR)/header.h
	$(CC) -c $(CDIR)/aux.c -o $(BDIR)/aux.o $(CFLAGS)

vis: vis.o
	$(CC) -o $(BDIR)/vis $(BDIR)/vis.o $(CFLAGS)

vis.o: $(CDIR)/vis.c
	$(CC) -c $(CDIR)/vis.c -o $(BDIR)/vis.o $(CFLAGS)

clean:
	rm $(BDIR)/*.o $(BDIR)/lab1 $(BDIR)/vis
