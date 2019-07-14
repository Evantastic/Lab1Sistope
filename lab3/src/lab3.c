#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "header.h"

int main(int argc, char **argv)
{
        struct command_line_args *args;
        args = proccess_args(argc, argv);
        read_files(args);
        fprintf(stderr, "OK\n");
        return 0;
}
