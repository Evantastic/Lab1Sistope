#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#define GET_FILE(x, y) y == UP_FILE ? x->up_traffic_file: \
                          y == DOWN_FILE ? x->down_traffic_file:\
                          x->usual_traffic_file
#define MAXLENINPUT 100
#define UP_FILE 0
#define DOWN_FILE 1
#define USUAL_FILE 2

static char read_file(struct command_line_args *args, int file_number) {
        FILE *file = GET_FILE(args, file_number);
        struct file_instruction instruction;
        char input[MAXLENINPUT];
        while (fgets(input, MAXLENINPUT, file) != NULL) {
                sscanf(input, "%u,%d,%d,%c,%u\n",
                                &instruction.time,
                                &instruction.initial_floor,
                                &instruction.destination_floor,
                                &instruction.direction,
                                &instruction.passenger_quantity);
        }
        return TRUE;
}

char read_files(struct command_line_args *args) {
       read_file(args, UP_FILE); 
       read_file(args, DOWN_FILE); 
       read_file(args, USUAL_FILE); 
       return TRUE;
}
