#include "header.h"

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
  threads = createStartThreads(monitors, *options);
  readFile(*options, monitors);
  waitThreads(threads, *options);
  printData(*options);
  freeData(monitors, options, threads);
	fprintf(stderr, "Ejecución completada\n");
	return 0;
}
