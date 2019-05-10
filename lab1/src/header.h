#ifndef HEADER_H
#define HEADER_H
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
  double realAverage, imgAverage, power, noise;
  int quantity;
};

/*
 * Funcion que se encarga de leer el achivo de entrada y analizar cada linea para ver a que proceso es enviado
 * Recibe los parametros entregados por  linea de comandos, el archivo de entrada y el arreglo de procesos creados
 * No posee retorno
 */
void readFile(struct flags *, FILE *, struct process *);

/*
 * Funcion que se encarga de liberar la memoria
 * Recibe los parametros entregados por  linea de comandos y el arreglo de procesos crados
 * No posee retorno
 */
void freeMemory(struct flags *, struct process *);

/*
 * Funcion que se encarga de cerrar los archivos
 * Recibe el arreglo de archivos abiertos
 * No posee retorno
 */
void closeFiles(FILE **);

/*
 * Funcion que se encarga de imprimir las visibilidades procesadas
 * Recibe los parametros entregados por  linea de comandos y el arreglo de procesos crados
 * No posee retorno
 */
void printQuantities(struct flags *, struct process *);

/*
 * Funcion que se encarga de imprimir las estadisticas en el archivo de salida
 * Recibe los parametros entregados por  linea de comandos, el archivo de salida y el arreglo de procesos crados
 * No posee retorno
 */
void printStatistics(struct flags *, FILE *, struct process *);

/*
 * Funcion que se encarga de obtener las estadisticas desde los procesos creados
 * Recibe los parametros entregados por  linea de comandos y el arreglo de procesos crados
 * Retorna el arreglo de procesos editados
 */
struct process *getStatistics(struct flags *, struct process *);

/*
 * Funcion que se encarga de verificar de si los parametros ingresados por linea de comandos son validos
 * Recibe los parametros entregados por  linea de comandos
 * Retorna un char correspontiente a FALSE o TRUE
 */
char checkOptions(struct flags*);

/*
 * Funcion que se encarga de obtener los parametros de la linea de comandos
 * Recibe los parametros recibidos en main, un entero que indica la cantidad de argumentos y un char ** que corraeponde al arreglo de strings entregado al programa
 * Retorna un puntero a la estructura que almacena los procesos
 */
struct flags *processArgv(int, char **);

/*
 * Estructura que se encarga de abrir los archivos
 * Recibe los parametros entregados por  linea de comandos
 * Retorna un arreglo de FILE*
 */
FILE **openFiles(struct flags *);

/*
 * Funcion que se encarga de crear los procesos y cambiar el codygo en memoria por el proceso vis
 * Recibe los parametros entregados por  linea de comandos
 * Entrega un arreglo de procesos
 */
struct process *createProcess(struct flags *);

#endif
