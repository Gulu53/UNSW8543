/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Reads, from a file whose name is given as command line         *
 *              argument, words from right to left, skipping some words        *
 *              according to the digits read from left to right, and           *
 *              outputs the words in a file named output.txt                   *
 *                                                                             *
 * Written by Eric Martin and *** for COMP9021                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void write_to_output (FILE* input_file, FILE* output_file, int word_count); 

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("I expect one command line argument (the name of a file).\n");
        return EXIT_FAILURE;
    }
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("I could not find the file %s.\n", argv[1]);
        return EXIT_FAILURE;
    }
    FILE *output_file = fopen("output.txt", "w");
    int word_count = 0, current_char = 0;
    for(long i = 0l; (current_char = getc(input_file)) != EOF; i++) {
        if (isdigit(current_char)) {
            word_count += (current_char - '0' + 1);
            fseek(input_file, 0l, SEEK_END);
            write_to_output(input_file, output_file, word_count);
            fseek(input_file, i + 1 ,SEEK_SET);
        }
    } 
    fclose(input_file);
    fclose(output_file);
    return EXIT_SUCCESS;
}
void write_to_output (FILE* input_file, FILE* output_file, int word_count) {
    long file_size = ftell(input_file);
    char newline = '\n';
    short int counter = 0;
    bool seq_start = false;
    for (long i = 2l; i <= file_size; i++) {
        fseek(input_file, -i, SEEK_END);
        if (seq_start) {
            if (!(isalpha(getc(input_file)))) {                
                seq_start = false;
                counter++;
                if (counter == word_count) {
                    fseek(input_file, -(i - 1), SEEK_END);
                    int current_char = 0;
                    while (isalpha(current_char = getc(input_file)))
                        fwrite(&current_char, 1, sizeof(char), output_file);
                    fwrite(&newline, 1, sizeof(char), output_file);
                    break;
                }
            }
        }
        else {
            if (isalpha(getc(input_file)))
                seq_start = true;
        }
    }
}
