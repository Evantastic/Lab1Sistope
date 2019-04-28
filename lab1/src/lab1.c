#include <stdio.h>
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
  //Datos de estadistica
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
    else if(viss[i].pid == 0){
      close(viss[i].pipes[FIN][OUTPUT]);
      close(viss[i].pipes[FOUT][INPUT]);
    }
    else{
      close(viss[i].pipes[SIN][OUTPUT]);
      close(viss[i].pipes[SOUT][INPUT]);
      dup2(viss[i].pipes[SIN][INPUT], STDIN_FILENO);
      dup2(viss[i].pipes[SOUT][OUTPUT], STDOUT_FILENO);
      execl("./bin/vis","./bin/vis",(char *) NULL);
      printf("ERROR\n");
    }
  }
  return viss;
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

void readFile(struct flags *options, FILE *input, struct process *viss){
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
  printOptions(options);
  viss = createProcess(options);
  if(viss == NULL){
    ERREXIT("Error creando procesos", 3);
  }
  //Verificar que viss no es nulo
  readFile(options, files[INPUT], viss);
  ENDOUTPUTS(options->discQuantity, viss);
  //Aca se reciben las estadisticas
  ENDINPUTS(options->discQuantity, viss);
  WAITPIDS(options->discQuantity, viss, &status);
  //Aca se imprime en el archivo
  //Aca se imprime si log == TRUE
  //Aca se libera memoria
  EXIT("Ejecucion completada", 0);
}
