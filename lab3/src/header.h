#ifndef HEADER_H
#define HEADER_H

#define TRUE 1
#define FALSE 0

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

char read_files(struct command_line_args *);

#endif
