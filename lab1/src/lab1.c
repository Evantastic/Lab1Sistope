#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

//Constantes
#define MAXLENSTRINGINPUT 100
#define MAXLENBUFFER 1000
#define TRUE 1
#define FALSE 0
#define INPUT 0
#define OUTPUT 1
#define FIN 0
#define FOUT 1
#define SIN 1
#define SOUT 0
//Macros
//Macro encargado de imprimir un mensaje de salida con un codigo y retornar el codigo
#define EXIT(x, y) fprintf(stderr, "%s. Terminado con codigo %d\n", x, y); return y
//Macro encargado de cerrar todos los pipes de escritura usados por este proceso
#define ENDOUTPUTS(n, x) for(int i = 0; i < n; i++) close(x[i].pipes[FOUT][OUTPUT])
//Macro encargado de cerrar todos los pipes de lectura usados por este proceso
#define ENDINPUTS(n, x) for(int i = 0; i < n; i++) close(x[i].pipes[FIN][INPUT])
//Macro encargado de esperar que todos los procesos creados terminen
#define WAITPIDS(n, x, y) for(int i = 0; i < n; i++) waitpid(x[i].pid, y, 0)
//Macro encargado de calcular a que disco pertenece una visibilidad
#define DISC(x, y, z) sqrt(pow(x, 2) + pow(y, 2)) / z

/*
 * Estrutura para representar los parametros ingresados por linea de comandos
 */
struct flags{
  char inputFile[MAXLENSTRINGINPUT];
  char outputFile[MAXLENSTRINGINPUT];
  int discQuantity;
  int discRadium;
  char log;
};

/*
 * Estrutura que representa un proceso creado
 */
struct process{
  int pipes[2][2];
  int pid;
  double mediaReal, mediaImg, potencia, noise;
  int cantidad;
};

/*
 * Funcion que se encarga de obtener los parametros de la linea de comandos
 * Recibe los parametros recibidos en main, un entero que indica la cantidad de argumentos y un char ** que corraeponde al arreglo de strings entregado al programa
 * Retorna un puntero a la estructura que almacena los procesos
 */
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

/*
 * Funcion que se encarga de crear los procesos y cambiar el codygo en memoria por el proceso vis
 * Recibe los parametros entregados por  linea de comandos
 * Entrega un arreglo de procesos
 */
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
      execl("./vis","./vis",(char *) NULL);
      return NULL;
    }
  }
  return viss;
}

/*
 * Estructura que se encarga de abrir los archivos
 * Recibe los parametros entregados por  linea de comandos
 * Retorna un arreglo de FILE*
 */
FILE **openFiles(struct flags *options){
  FILE **files;
  files = (FILE **) calloc(2, sizeof(FILE *));
  files[0] = fopen(options->inputFile, "r");
  files[1] = fopen(options->outputFile, "w");
  if(files[0] == NULL || files[1] == NULL)
    return NULL;
  return files;
}

/*
 * Funcion que se encarga de verificar de si los parametros ingresados por linea de comandos son validos
 * Recibe los parametros entregados por  linea de comandos
 * Retorna un char correspontiente a FALSE o TRUE
 */
char checkOptions(struct flags *options){
  char state = TRUE;
  state = state && options->inputFile;
  state = state && options->outputFile;
  state = state && options->discQuantity > 0;
  state = state && options->discRadium > 0;
  return state;
}

/*
 * Funcion que se encarga de leer el achivo de entrada y analizar cada linea para ver a que proceso es enviado
 * Recibe los parametros entregados por  linea de comandos, el archivo de entrada y el arreglo de procesos creados
 * No posee retorno
 */
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

/*
 * Funcion que se encarga de obtener las estadisticas desde los procesos creados
 * Recibe los parametros entregados por  linea de comandos y el arreglo de procesos crados
 * Retorna el arreglo de procesos editados
 */
struct process *getStatistics(struct flags *options, struct process *viss){
  char buffer[MAXLENBUFFER];
  for(int i = 0; i < options->discQuantity; i++){
    memset(buffer, 0, MAXLENBUFFER);
    read(viss[i].pipes[FIN][INPUT], buffer, MAXLENBUFFER);
    sscanf(buffer,"%lf,%lf,%lf,%lf,%d", &viss[i].mediaReal, &viss[i].mediaImg, &viss[i].potencia, &viss[i].noise, &viss[i].cantidad);
  }
  return viss;
}

/*
 * Funcion que se encarga de imprimir las estadisticas en el archivo de salida
 * Recibe los parametros entregados por  linea de comandos, el archivo de salida y el arreglo de procesos crados
 * No posee retorno
 */
void printStatistics(struct flags *options, FILE *output, struct process *viss){
  for(int i = 0; i < options->discQuantity; i++){
    fprintf(output,"Disco %d\n",i+1);
    fprintf(output, "Media Real: %lf\nMedia Imaginaria: %lf\nPotencia: %lf\nRuido Total: %lf\n", viss[i].mediaReal, viss[i].mediaImg, viss[i].potencia, viss[i].noise);
  }
}

/*
 * Funcion que se encarga de imprimir las visibilidades procesadas
 * Recibe los parametros entregados por  linea de comandos y el arreglo de procesos crados
 * No posee retorno
 */
void printQuantities(struct flags *options, struct process *viss){
  if(!options->log)
    return;
  for(int i = 0; i < options->discQuantity; i++){
    printf("Proceso con PID %d procesa %d visibilidades\n", viss[i].pid, viss[i].cantidad);
  }
}

/*
 * Funcion que se encarga de liberar la memoria
 * Recibe los parametros entregados por  linea de comandos y el arreglo de procesos crados
 * No posee retorno
 */
void freeMemory(struct flags *options, struct process *viss){
  free(options);
  free(viss);
}

/*
 * Funcion que se encarga de cerrar los archivos
 * Recibe el arreglo de archivos abiertos
 * No posee retorno
 */
void closeFiles(FILE **files){
  fclose(files[INPUT]);
  fclose(files[OUTPUT]);
}

int main(int argc, char **argv){
  struct flags *options;
  FILE **files;
  struct process *viss;
  int status;

  options = processArgv(argc, argv);
  files = openFiles(options);
  if(options == NULL){
    EXIT("Error procesando los parametros", 1);
  }
  else if(!checkOptions(options)){
    EXIT("Error procesando los parametros", 1);
  }
  else if(files == NULL){
    EXIT("Error procesando archivos", 2);
  }
  viss = createProcess(options);
  if(viss == NULL){
    EXIT("Error creando procesos", 3);
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
