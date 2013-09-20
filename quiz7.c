/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Processes 6 command line arguments, modifying them and         *
 *              shuffling them around.                                         *
 *                                                                             *
 * Written by Eric Martin and *** for COMP9021                                 *
 *                                                                             *
 * Other source files, if any, one per line, starting on the next line:        *
 *       my_code.c                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>

#define NB_OF_ARGS 6

void process_command_line_arguments(char **, int);

int main(int argc, char **argv) {
    if (argc != NB_OF_ARGS + 1) {
        printf("Run the program with 6 command line arguments please.\n");
        return EXIT_FAILURE;
    }
    process_command_line_arguments(argv, argc);
    for (int i = 1; i <= NB_OF_ARGS; ++i)
        printf("  %s", argv[i]);
    putchar('\n');
    return EXIT_SUCCESS;
}

