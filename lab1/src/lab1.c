#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLENSTRINGINPUT 100
#define MAXLENBUFFER 1000
#define TRUE 1
#define FALSE 0
#define ERREXIT(x, y) fprintf(stderr, "%s. Terminado con codigo %d\n", x, y); return y
#define EXIT(x, y) fprintf(stderr, "%s. Terminado con codigo %d\n", x, y); return y
#define INPUT 0
#define OUTPUT 1
#define FIN 0
#define FOUT 1
#define SIN 1
#define SOUT 0
#define ENDOUTPUTS(n, x) for(int i = 0; i < n; i++) close(x[i].pipes[FOUT][OUTPUT])
#define ENDINPUTS(n, x) for(int i = 0; i < n; i++) close(x[i].pipes[FIN][INPUT])
#define WAITPIDS(n, x, y) for(int i = 0; i < n; i++) waitpid(x[i].pid, y, 0)
#define DISC(x, y, z) sqrt(pow(x, 2) + pow(y, 2)) / z

struct flags{
  char inputFile[MAXLENSTRINGINPUT];
  char outputFile[MAXLENSTRINGINPUT];
  int discQuantity;
  int discRadium;
  char log;
};

struct process{
  int pipes[2][2];
  int pid;
  double mediaReal, mediaImg, potencia, noise;
  int cantidad;
};

struct flags *processArgv(int argc, char **argv){
  int currentFlag = 0;
  struct flags *options = (struct flags *) calloc(1, sizeof(struct flags));
  while((currentFlag = getopt(argc, argv, "i:o:d:n:b")) != -1){
    switch(currentFlag){
      case 'i':
        strcpy(options->inputFile, optarg);
        break;
      case 'o':
        strcpy(options->outputFile, optarg);
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
  return options;
}

struct process *createProcess(struct flags *options){
  struct process *viss = (struct process *) calloc(options->discQuantity, sizeof(struct process));
  for(int i = 0; i < options->discQuantity; i++){
    pipe(viss[i].pipes[0]);
    pipe(viss[i].pipes[1]);
    viss[i].pid = fork();
    if(viss[i].pid < 0){
      return NULL;
    }
    else if(viss[i].pid > 0){
      close(viss[i].pipes[FIN][OUTPUT]);
      close(viss[i].pipes[FOUT][INPUT]);
    }
    else{
      close(viss[i].pipes[SIN][OUTPUT]);
      close(viss[i].pipes[SOUT][INPUT]);
      dup2(viss[i].pipes[SIN][INPUT], STDIN_FILENO);
      dup2(viss[i].pipes[SOUT][OUTPUT], STDOUT_FILENO);
      execl("./bin/vis","./bin/vis",(char *) NULL);
      return NULL;
    }
  }
  return viss;
}

FILE **openFiles(struct flags *options){
  FILE **files;
  files = (FILE **) calloc(2, sizeof(FILE *));
  files[0] = fopen(options->inputFile, "r");
  files[1] = fopen(options->outputFile, "w");
  if(files[0] == NULL || files[1] == NULL)
    return NULL;
  return files;
}

char checkOptions(struct flags *options){
  char state = TRUE;
  state = state && options->inputFile;
  state = state && options->outputFile;
  state = state && options->discQuantity > 0;
  state = state && options->discRadium > 0;
  return state;
}

void readFile(struct flags *options, FILE *input, struct process *viss){
  float u, v, r, i, n;
  int position;
  char buffer[MAXLENBUFFER];
  while(fgets(buffer, MAXLENBUFFER, input) != NULL){
    sscanf(buffer, "%f,%f,%f,%f,%f\n", &u, &v, &r, &i, &n);
    position = floor(DISC(u, v, options->discRadium));
    position = position >= options->discQuantity ? options->discQuantity - 1: position;
    write(viss[position].pipes[FOUT][OUTPUT], buffer, strlen(buffer));
  }
}

struct process *getStatistics(struct flags *options, struct process *viss){
  char buffer[MAXLENBUFFER];
  for(int i = 0; i < options->discQuantity; i++){
    memset(buffer, 0, MAXLENBUFFER);
    read(viss[i].pipes[FIN][INPUT], buffer, MAXLENBUFFER);
    sscanf(buffer,"%lf,%lf,%lf,%lf,%d", &viss[i].mediaReal, &viss[i].mediaImg, &viss[i].potencia, &viss[i].noise, &viss[i].cantidad);
  }
  return viss;
}

void printStatistics(struct flags *options, FILE *output, struct process *viss){
  for(int i = 0; i < options->discQuantity; i++){
    fprintf(output,"Disco %d\n",i+1);
    fprintf(output, "Media Real: %lf\nMedia Imaginaria: %lf\nPotencia: %lf\nRuido Total: %lf\n", viss[i].mediaReal, viss[i].mediaImg, viss[i].potencia, viss[i].noise);
  }
}

void printQuantities(struct flags *options, struct process *viss){
  if(!options->log)
    return;
  for(int i = 0; i < options->discQuantity; i++){
    printf("Proceso con PID %d procesa %d visibilidades\n", viss[i].pid, viss[i].cantidad);
  }
}

void freeMemory(struct flags *options, struct process *viss){
  free(options);
  free(viss);
}

void closeFiles(FILE **files){
  fclose(files[INPUT]);
  fclose(files[OUTPUT]);
}

/*
 * -i nombre de archivo de entrada
 * -o nombre de archivo de salida
 * -d ancho discos
 * -n ndiscos
 * -b
 */
int main(int argc, char **argv){
  struct flags *options;
  FILE **files;
  struct process *viss;
  int status;

  options = processArgv(argc, argv);
  files = openFiles(options);
  if(options == NULL){
    ERREXIT("Error procesando los parametros", 1);
  }
  else if(!checkOptions(options)){
    ERREXIT("Error procesando los parametros", 1);
  }
  else if(files == NULL){
    ERREXIT("Error procesando archivos", 2);
  }
  viss = createProcess(options);
  if(viss == NULL){
    ERREXIT("Error creando procesos", 3);
  }
  readFile(options, files[INPUT], viss);
  ENDOUTPUTS(options->discQuantity, viss);
  getStatistics(options, viss);
  ENDINPUTS(options->discQuantity, viss);
  WAITPIDS(options->discQuantity, viss, &status);
  printStatistics(options, files[OUTPUT], viss);
  printQuantities(options, viss);
  freeMemory(options, viss);
  closeFiles(files);
  EXIT("Ejecucion completada", 0);
}
