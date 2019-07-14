#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "header.h"
#define ERROR_MSG_3 "Too many command line arguments\n"
#define ERROR_MSG_4 "Error accessing the indicated files\n"
#define ERROR_MSG_5 "Error processing command line arguments\n"

/*
   * Funcion que dado una bandera, establece el valor en una estructura
   * Recibe el puntero a la estructura de los argumentos de linea de comandos
   y la bandera a evaluar 
   * En caso de ser una bandera valida, se retorna TRUE, en caso contrario
   se retorna FALSE
   */
static char set_valid_flag(struct command_line_args *args, char current_flag)
{
        if (current_flag == 'u') 
                args->up_traffic_file = fopen(optarg, "r");
        else if (current_flag == 'd')
                args->down_traffic_file = fopen(optarg, "r");
        else if (current_flag == 'o')
                args->usual_traffic_file = fopen(optarg, "r");
        else if (current_flag == 'n')
                args->floor_quantity = atoi(optarg);
        else if (current_flag == 'e')
                args->elevator_quantity = atoi(optarg);
        else if (current_flag == 'b')
                args->log = TRUE;
        else
                return FALSE;
        return TRUE;
}

/*
   * Funcion que dado los argumentos de linea de comandos, verifica que todos
   hayan sido ingresados correctamente, no se admiten valores nulos
   * Recibe los argumentos de linea de comandos
   * En caso de ser valida, retorna TRUE, en caso contrario, retorna FALSE
   */
static char is_command_line_args(const struct command_line_args *args)
{
        char state = TRUE;
        state = state && args->up_traffic_file;
        state = state && args->down_traffic_file;
        state = state && args->usual_traffic_file;
        state = state && args->floor_quantity;
        state = state && args->elevator_quantity;
        return state;
}

/*
   * Funcion que valida que los argumentos de linea de comandos sea valida,
   verifica que los archivos que se hayan entregado existan y que todos los
   valores necesarios hayan sido entregador
   * Recibe los argumentos de linea de comandos
   * En caso de ser valido, la ejecucion continua, en caso contrario, la
   ejecucion se ve terminada
   */
static void validate_command_line_args(const struct command_line_args *args)
{
        if (args->up_traffic_file == NULL ||
                        args->down_traffic_file == NULL ||
                        args->usual_traffic_file == NULL) {
                fprintf(stderr, ERROR_MSG_4);
                exit(4);
        } else if (!is_command_line_args(args)) {
                fprintf(stderr, ERROR_MSG_5);
                exit(5);
        }
}

/*
   * Funcion que obtiene los argumentos de linea de comando, verificando
   que se hayan entregado todos los argumentos, que se haya accedido a los
   archivos, y que no se hayan entregado argumentos demas.
   * Recibe la cantidad de argumentos fueron ingresados por la linea de
   comandos, incluyendo el titulo, y el arreglo de comandos ingresado por
   linea de comandos, incluyendo el titulo
   * En caso de que todo se haya validado correctamente, se retorna el
   puntero a la estructura, en caso contrario, la ejecucion es abortada
   */
struct command_line_args *proccess_args(int argc, char **argv)
{
        int current_flag;
        struct command_line_args *args;
        extern int opterr;
        opterr = 0;
        args = (struct command_line_args *) calloc(
                        1,
                        sizeof(struct command_line_args)
                        );
        while ((current_flag = getopt(argc, argv, "u:d:o:n:e:b")) != -1) {
                if(!set_valid_flag(args, current_flag)) {
                        fprintf(stderr, ERROR_MSG_3);
                        exit(3);
                }
        }
        validate_command_line_args(args);
        return args;
}

