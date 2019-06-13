#include "header.h"

/*
 * Función que procesa las entradas
 * Entrada: cantidad de elementos por linea de comandos y los parametros ingresados
 * Salida: estructura que contiene las opciones
 */
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

/*
 * Función que verifica las opciones ingresadas
 * Entrada: opciones ingresadas
 * Salida: TRUE/FALSE
 */
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

/*
 * Función que crea la cantidad designada de monitores
 * Entrada: opciones ingresadas
 * Salida: structura con los monitores correspondientes
 */
struct monitor *createMonitors(const struct flags options)
{
  struct monitor *monitors = (struct monitor *) calloc(options.discQuantity, sizeof(struct monitor));
  for (int i = 0; i < options.discQuantity; i++) {
    monitors[i].bufferSize = options.bufferSize;
    monitors[i].actualSize = 0;
    monitors[i].discNumber = i;
    monitors[i].buffer = (struct data *) calloc(options.bufferSize, sizeof(struct data));
    monitors[i].lock = (pthread_mutex_t *) calloc(1, sizeof(pthread_mutex_t));
    monitors[i].emptyBuffer = (pthread_cond_t *) calloc(1, sizeof(pthread_cond_t));
    monitors[i].fullBuffer = (pthread_cond_t *) calloc(1, sizeof(pthread_cond_t));
    pthread_mutex_init(monitors[i].lock, NULL);
    pthread_cond_init(monitors[i].emptyBuffer, NULL);
    pthread_cond_init(monitors[i].fullBuffer, NULL);
  }
  return monitors;
}

/*
 * Proceso que libera la memoria del programa
 * Entrada: los monitores, las opciones y las hebras
 */
void freeData(struct monitor *monitors, struct flags *options, pthread_t *threads)
{
  for (int i = 0; i < options->discQuantity; i++) {
    free(monitors[i].lock);
    free(monitors[i].fullBuffer);
    free(monitors[i].emptyBuffer);
    free(monitors[i].buffer);
  }
  free(monitors);
  free(threads);
  free(options);
}

/*
 * Función que crea e inicializa las hebras
 * Entrada: monitores y las opciones
 * Salida: las estructuras de las hebras inicializadas
 */
pthread_t *createStartThreads(const struct monitor *monitors, const struct flags options)
{
  pthread_t *threads = (pthread_t *) calloc(options.discQuantity, sizeof(pthread_t));
  for (int i = 0; i < options.discQuantity; i++) {
    pthread_create(&threads[i], NULL, readData, (void *) &monitors[i]);
  }
  return threads;
}

/*
 * Proceso que espera a que las hebras terminen su ejecucion
 * Entrada: las hebras y las opciones
 */
void waitThreads(const pthread_t *threads, const struct flags options)
{
  for (int i = 0; i < options.discQuantity; i++) {
    pthread_join(threads[i], NULL);
  }
}