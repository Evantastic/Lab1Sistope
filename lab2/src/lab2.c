#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

struct flags {
	FILE *inputFile;
	FILE *outputFile;
	int discQuantity;
	int discRadium;
	char log;
};

struct flags *proccessArgv(int argc, char **argv)
{
	int currentFlag;
	struct flags *options = (struct flags *) calloc(1, sizeof(struct flags));
	while ((currentFlag = getopt(argc, argv, "i:o:d:n:b")) != -1) {
		switch (currentFlag) {
			case 'i':
				options->inputFile = fopen(optarg, "r");
				break;
			case 'o':
				options->outputFile = fopen(optarg,"w");
				break;
			case 'd':
				options->discRadium = atoi(optarg);
				break;
			case 'n': 
				options->discQuantity = atoi(optarg);
				break;
			case 'b':
				options->log = TRUE;
				break;
			default:
				return NULL;
		}
	}
	if (options->inputFile == NULL || options->outputFile == NULL)
		return NULL;
	return options;
}

char checkOptions(const struct flags *options)
{
	char state = TRUE;
	state = state && options->inputFile;
	state = state && options->outputFile;
	state = state && options->discQuantity;
	state = state && options->discRadium;
	return state;
}

int main(int argc, char **argv)
{
	struct flags *options;
	options = proccessArgv(argc, argv);
	if (options == NULL) {
		fprintf(stderr, "Error, parámetros erróneos\n");
		exit(1);
	} else if (!checkOptions(options)) {
		fprintf(stderr, "Error, parámetros erróneos\n");
		exit(1);
	}
	fprintf(stderr, "Ejecución completada\n");
	return 0;
}
