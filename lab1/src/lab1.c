#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAXLENSTRINGINPUT 100
#define TRUE 1
#define FALSE 0
#define ERREXIT(x, y) fprintf(stderr, "%s. Terminado con codigo %d\n", x, y); return y
#define EXIT(x, y) fprintf(stderr, "%s. Terminado con codigo %d\n", x, y); return y
#define INPUT 0
#define OUTPUT 1

struct flags{
  char inputFile[MAXLENSTRINGINPUT];
  char outputFile[MAXLENSTRINGINPUT];
  int discQuantity;
  int discRadium;
  char log;
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

void printOptions(struct flags *options){
  fprintf(stdout, "Input: %s\n", options->inputFile);
  fprintf(stdout, "Output: %s\n", options->outputFile);
  fprintf(stdout, "Cantidad de discos: %d\n", options->discQuantity);
  fprintf(stdout, "Ancho de discor: %d\n", options->discRadium);
  fprintf(stdout, "Log: %s\n", options->log ? "True" : "False");
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

void readFile(struct flags *options, FILE *input){
  float u, v, r, i, n;
  int index = 0;
  while(fscanf(input, "%f,%f,%f,%f,%f\n", &u, &v, &r, &i, &n) != EOF){
    index++;
    fprintf(stderr, "Linea %d\t:u=%f,\tv=%f,\ti=%f,\tr=%f,\tn=%f\n", index, u, v, r, i, n);
  }
}
/*
 * -i nombre de archivo de entrada
 * -o nombre de archivo de salida
 * -d ancho discos
 * -n ndiscos
 * -b
 */
int main(int argc, char **argv){
  struct flags *options = processArgv(argc, argv);
  FILE **files = openFiles(options);
  if(options == NULL){
    ERREXIT("Error procesando los parametros", 1);
  }
  else if(files == NULL){
    ERREXIT("Error procesando archivos", 2);
  }
  printOptions(options);
  readFile(options, files[INPUT]);
  EXIT("Ejecucion completada", 0);
}
