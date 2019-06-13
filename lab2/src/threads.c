#include "header.h"

pthread_mutex_t *lock;
char reading = TRUE;
char logNeeded = FALSE;
struct globalData sharedData;

/*
 * Proceso que inicializa los datos a usar por las hebras
 * Entrada: las opciones ingresadas
 */
void static initializeData(const struct flags options)
{
  lock = (pthread_mutex_t *) calloc(1, sizeof(pthread_mutex_t));
  sharedData.imgMean = (float *) calloc(options.discQuantity, sizeof(float));
  sharedData.realMean = (float *) calloc(options.discQuantity, sizeof(float));
  sharedData.noise = (float *) calloc(options.discQuantity, sizeof(float));
  sharedData.power = (float *) calloc(options.discQuantity, sizeof(float));
  pthread_mutex_init(lock, NULL);
  if (options.log)
    logNeeded = TRUE;
}


/*
 * Proceso que se encarga de ejecutar la logica solicitada para las hebras
 * Entrada: recibe el monitor asociado a la hebra
 */
void *readData(void *data)
{
  struct monitor *monitor = (struct monitor *) data;
  int quantity = 0;
  float imgMean = 0.0;
  float realMean = 0.0;
  float noise = 0.0;
  float power = 0.0;

  while (reading) {
    pthread_mutex_lock(monitor->lock);
    if (monitor->actualSize != monitor->bufferSize) {
      pthread_cond_wait(monitor->fullBuffer, monitor->lock);
    }
    for (int i = 0; i < monitor->actualSize; i++) {
      imgMean += monitor->buffer[i].i;
      realMean += monitor->buffer[i].r;
      power += sqrt(pow(monitor->buffer[i].r, 2) + pow(monitor->buffer[i].i, 2));
      noise += monitor->buffer[i].n;
      quantity++;
    }
    monitor->actualSize = 0;
    pthread_cond_signal(monitor->emptyBuffer);
    pthread_mutex_unlock(monitor->lock);
  }
  for (int i = 0; i < monitor->actualSize; i++) {
    imgMean += monitor->buffer[i].i;
    realMean += monitor->buffer[i].r;
    power += sqrt(pow(monitor->buffer[i].r, 2) + pow(monitor->buffer[i].i, 2));
    noise += monitor->buffer[i].n;
    quantity++;
  }
  if (logNeeded)
    fprintf(stdout, "Hebra con id %d procesó %d visibilidades\n", monitor->discNumber, quantity);
  pthread_mutex_lock(lock);
  sharedData.imgMean[monitor->discNumber] = imgMean / (float) quantity;
  sharedData.realMean[monitor->discNumber] = realMean / (float) quantity;
  sharedData.noise[monitor->discNumber] = noise;
  sharedData.power[monitor->discNumber] = power;
  pthread_mutex_unlock(lock);
  return NULL;
}

/*
 * Proceso que lee el archivo y envia los datos a las hebras
 * Entrada: las opciones ingresadas y los monitores
 */
void readFile(struct flags options, struct monitor *monitors)
{
  float u, v, r, i, n;
  int position;
  char buffer[MAXLENBUFFER];
  
  initializeData(options);
  while (fgets(buffer, MAXLENBUFFER, options.inputFile) != NULL) {
    sscanf(buffer, "%f, %f, %f, %f, %f\n", &u, &v, &r, &i, &n);
    position = DISC(u, v, options.discRadium);
    position = position >= options.discQuantity ? options.discQuantity - 1: position;
    pthread_mutex_lock(monitors[position].lock);
    if( monitors[position].actualSize == monitors[position].bufferSize) {
      pthread_cond_signal(monitors[position].fullBuffer);
      pthread_cond_wait(monitors[position].emptyBuffer, monitors[position].lock);
    }
    monitors[position].buffer[monitors[position].actualSize].u = u;
    monitors[position].buffer[monitors[position].actualSize].v = v;
    monitors[position].buffer[monitors[position].actualSize].r = r;
    monitors[position].buffer[monitors[position].actualSize].i = i;
    monitors[position].buffer[monitors[position].actualSize].n = n;
    monitors[position].actualSize++;
    pthread_mutex_unlock(monitors[position].lock);
  }
  reading = FALSE;
  for (int i = 0; i < options.discQuantity; i++) {
    pthread_cond_signal(monitors[i].fullBuffer);
  }
}

/*
 * Proceso que imprime los datos obtenidos por las hebras
 * Entrada: las opciones ingresadas
 */
void printData(const struct flags options)
{
  for(int i = 0; i < options.discQuantity; i++) {
    fprintf(options.outputFile,"Disco %d\n",i+1);
    fprintf(options.outputFile, "Media Real: %f\n", sharedData.realMean[i]);
    fprintf(options.outputFile, "Media Imaginaria: %f\n", sharedData.imgMean[i]);
    fprintf(options.outputFile, "Potencia: %f\n", sharedData.power[i]);
    fprintf(options.outputFile, "Ruido Total: %f\n", sharedData.noise[i]);
  }
}