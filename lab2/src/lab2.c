#include "header.h"

struct flags *proccessArgv(int argc, char **argv)
{
	int currentFlag;
	struct flags *options = (struct flags *) calloc(1, sizeof(struct flags));
	while ((currentFlag = getopt(argc, argv, "i:o:d:n:s:b")) != -1) {
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
			case 's':
				options->bufferSize = atoi(optarg);
				break;
			default:
				return NULL;
		}
	}
	if (options->inputFile == NULL || options->outputFile == NULL)
		return NULL;
	return options;
}

char checkOptions(const struct flags options)
{
	char state = TRUE;
	state = state && options.inputFile;
	state = state && options.outputFile;
	state = state && options.discQuantity;
	state = state && options.discRadium;
	state = state && options.bufferSize;
	return state;
}

struct monitor *createMonitors(const struct flags options)
{
  struct monitor *monitors = (struct monitor *) calloc(options.discQuantity, sizeof(struct monitor));
  for (int i = 0; i < options.discQuantity; i++) {
    monitors[i].bufferSize = options.bufferSize;
    monitors[i].actualSize = 0;
    monitors[i].discNumber = i;
    monitors[i].buffer = (struct data *) calloc(options.bufferSize, sizeof(struct data *));
    monitors[i].lock = (pthread_mutex_t *) calloc(1, sizeof(pthread_mutex_t));
    monitors[i].emptyBuffer = (pthread_cond_t *) calloc(1, sizeof(pthread_cond_t));
    monitors[i].fullBuffer = (pthread_cond_t *) calloc(1, sizeof(pthread_cond_t));
    pthread_mutex_init(monitors[i].lock, NULL);
    pthread_cond_init(monitors[i].emptyBuffer, NULL);
    pthread_cond_init(monitors[i].fullBuffer, NULL);
  }
  return monitors;
}


void *readData(void *data)
{
  struct monitor *monitor = (struct monitor *) data;
  while (reading) {
    fprintf(stderr, "Hebra %d comenzando captura de datos\n", monitor->discNumber);
    pthread_mutex_lock(monitor->lock);
    fprintf(stderr, "Hebra %d ingresa al buffer\n", monitor->discNumber);
    if (monitor->actualSize != monitor->bufferSize) {
      fprintf(stderr, "Hebra %d con buffer lleno\n", monitor->discNumber);
      pthread_cond_wait(monitor->fullBuffer, monitor->lock);
    }
    fprintf(stderr, "Hebra %d trabajando\n", monitor->discNumber);
    monitor->actualSize = 0;
    pthread_cond_signal(monitor->emptyBuffer);
    pthread_mutex_unlock(monitor->lock);
  }
  return NULL;
}

void readFile(struct flags options, struct monitor *monitors)
{
  float u, v, r, i, n;
  int linea = 0;
  int position;
  char buffer[MAXLENBUFFER];
  while (fgets(buffer, MAXLENBUFFER, options.inputFile) != NULL) {
    sscanf(buffer, "%f, %f, %f, %f, %f\n", &u, &v, &r, &i, &n);
    position = floor(DISC(u, v, options.discRadium));
    position = position >= options.discQuantity ? options.discQuantity - 1: position;
    pthread_mutex_lock(monitors[position].lock);
    fprintf(stderr, "Padre en disco %d leyendo %d\n", position, linea++);
    if( monitors[position].actualSize == monitors[position].bufferSize) {
      fprintf(stderr, "Padre en disco %d lleno\n", position);
      pthread_cond_signal(monitors[position].fullBuffer);
      pthread_cond_wait(monitors[position].emptyBuffer, monitors[position].lock);
    }
    fprintf(stderr, "Padre en disco %d no lleno, tamaño actual %d\n", position, monitors[position].actualSize+1);
    monitors[position].actualSize++;
    pthread_mutex_unlock(monitors[position].lock);
  }
  reading = FALSE;
  for (int i = 0; i < options.discQuantity; i++) {
    pthread_cond_signal(monitors[i].fullBuffer);
  }
}

pthread_t *createStartThreads(const struct monitor *monitors, const struct flags options)
{
  pthread_t *threads = (pthread_t *) calloc(options.discQuantity, sizeof(pthread_t));
  for (int i = 0; i < options.discQuantity; i++) {
    pthread_create(&threads[i], NULL, readData, (void *) &monitors[i]);
  }
  return threads;
}

void waitThreads(const pthread_t *threads, const struct flags options)
{
  for (int i = 0; i < options.discQuantity; i++) {
    pthread_join(threads[i], NULL);
  }
}

int main(int argc, char **argv)
{
	struct flags *options;
  struct monitor *monitors;
  pthread_t *threads;
	options = proccessArgv(argc, argv);
	if (options == NULL) {
		fprintf(stderr, "Error, parámetros erróneos\n");
		exit(1);
	} else if (!checkOptions(*options)) {
		fprintf(stderr, "Error, parámetros erróneos\n");
		exit(1);
	}
  monitors = createMonitors(*options);
  fprintf(stderr, "Comenzando creacion de hebras\n");
  threads = createStartThreads(monitors, *options);
  fprintf(stderr, "Comenzando lectura archivo\n");
  readFile(*options, monitors);
  waitThreads(threads, *options);
	fprintf(stdout, "Ejecución completada\n");
	return 0;
}
