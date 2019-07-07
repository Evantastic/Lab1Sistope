#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define IS_UP(x) x == 'U' ? 1: 0;
#define IS_DOWN(x) x == 'D' ? 1: 0;
#define TRUE 1
#define FALSE 0
#define ERROR_MSG "Error processing command line arguments\n"

/*
   * Estructura que representa los argumentos entregados por linea de comandos
   */
struct command_line_args {
        FILE *up_traffic_file;
        FILE *down_traffic_file;
        FILE *usual_traffic_file;
        unsigned int floor_quantity;
        unsigned int elevator_quantity;
        char log;
};

/*
   * Estructura que representa una llamada al ascensor
   */
struct file_instruction {
        unsigned int time;
        int initial_floor;
        int destination_floor;
        char direction;
        unsigned int passenger_quantity;
};

struct command_line_args *proccess_args(int argc, char **argv)
{
        int current_flag;
        struct command_line_args *args;

        args = (struct command_line_args *) calloc(
                        1,
                        sizeof(struct command_line_args)
                        );
        while ((current_flag = getopt(argc, argv, "u:d:o:n:e:b")) != -1) {
                switch (current_flag) {
                        case 'u':
                                args->up_traffic_file = fopen(optarg, "r");
                                break;
                        case 'd':
                                args->down_traffic_file = fopen(optarg, "r");
                                break;
                        case 'o':
                                args->usual_traffic_file = fopen(optarg, "r");
                                break;
                        case 'n':
                                args->floor_quantity = atoi(optarg);
                                break;
                        case 'e':
                                args->elevator_quantity = atoi(optarg);
                                break;
                        case 'b':
                                args->log = TRUE;
                                break;
                }
        }
        if (args->up_traffic_file == NULL ||
            args->down_traffic_file == NULL ||
            args->usual_traffic_file == NULL)
                return NULL;
        return args;
}

char is_command_line_args(const struct command_line_args *args)
{
        char state = TRUE;
        state = state && args->up_traffic_file;
        state = state && args->down_traffic_file;
        state = state && args->usual_traffic_file;
        state = state && args->floor_quantity;
        state = state && args->elevator_quantity;
        return state;
}

int main(int argc, char **argv)
{
        struct command_line_args *args;

        args = proccess_args(argc, argv);
        if (!is_command_line_args(args)) {
                fprintf(stderr, ERROR_MSG);
                exit(5);
        }
        fprintf(stderr, "OK\n");
        return 0;
}
