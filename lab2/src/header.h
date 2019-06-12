#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <pthread.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define MAXLENBUFFER 200
#define DISC(x, y, z) sqrt(pow(x, 2) + pow(y, 2)) / z

struct flags {
	FILE *inputFile;
	FILE *outputFile;
	int discQuantity;
	int discRadium;
	int bufferSize;
	char log;
};

struct data {
  float u;
  float v;
  float i;
  float r;
  float n;
};

struct monitor {
	int bufferSize;
	int actualSize;
	int discNumber;
	struct data *buffer;
	pthread_cond_t *fullBuffer;
	pthread_cond_t *emptyBuffer;
	pthread_mutex_t *lock;
};

pthread_mutex_t lock;

char reading = TRUE;

#endif