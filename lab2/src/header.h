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
#define DISC(x, y, z) floor(sqrt(pow(x, 2) + pow(y, 2)) / z)


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

struct globalData {
	float *realMean;
	float *imgMean;
	float *noise;
	float *power;
};



struct flags *proccessArgv(int argc, char **argv);

char checkOptions(const struct flags options);

struct monitor *createMonitors(const struct flags options);

void freeData(struct monitor *monitors, struct flags *options, pthread_t *threads);

void printData(const struct flags options);

void waitThreads(const pthread_t *threads, const struct flags options);

pthread_t *createStartThreads(const struct monitor *monitors, const struct flags options);

void readFile(struct flags options, struct monitor *monitors);

void *readData(void *data);

#endif