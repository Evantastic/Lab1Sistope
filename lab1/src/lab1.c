#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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

char checkOptions(struct flags *options){
  char state = TRUE;
  state = state && options->inputFile;
  state = state && options->outputFile;
  state = state && options->discQuantity > 0;
  state = state && options->discRadium > 0;
  return state;
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
  else if(!checkOptions(options)){
    ERREXIT("Error procesando los parametros", 1);
  }
  else if(files == NULL){
    ERREXIT("Error procesando archivos", 2);
  }
  printOptions(options);
  //readFile(options, files[INPUT]);

  int pid;
  int pipes[2][2];
  char buffer[MAXLENBUFFER];
  pipe(pipes[INPUT]);
  pipe(pipes[OUTPUT]);
  pid = fork();
  if(pid < 0){
    ERREXIT("Error creando procesos", 3);
  }
  else if(pid == 0){
    close(pipes[FIN][OUTPUT]);
    close(pipes[FOUT][INPUT]);
    write(pipes[FOUT][OUTPUT], "wenahijo\n", 10);
    read(pipes[FIN][INPUT], buffer, MAXLENBUFFER);
    fprintf(stderr, "Mi hijo me dijo: %s\n", buffer);
    close(pipes[FIN][INPUT]);
    close(pipes[FOUT][OUTPUT]);
    EXIT("Termine desde el padre", 0);
  }
  else{
    close(pipes[SIN][OUTPUT]);
    close(pipes[SOUT][INPUT]);
    dup2(pipes[SIN][INPUT], STDIN_FILENO);
    dup2(pipes[SOUT][OUTPUT], STDOUT_FILENO);
    fprintf(stdout, "wenapadre");
    fscanf(stdin, "%s", buffer);
    fprintf(stderr, "Mi padre me dijo: %s\n", buffer);
    close(pipes[SIN][INPUT]);
    close(pipes[SOUT][OUTPUT]);
    EXIT("Termine desde el hijo", 0);
  }

  EXIT("Ejecucion completada", 0);
}
