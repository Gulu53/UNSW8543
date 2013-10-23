/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:                                                                *
 *                                                                             *
 * Written by *** for COMP9021                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_sentences(int argc, char **argv);
void get_sentences(int argc, char **argv, int *sentences, int sentences_nb);
void get_variable_instances(char **argv, char **variable_instances, int variable_instances_nb);
bool keyword(char *, char**);
void sort(char**, int);

int main(int argc, char **argv) {
    int sentences_nb, variable_instances_nb, *sentences;
    char **variable_instances;
    //Count number of sentences present, so that number of equations can be determined. 
    sentences_nb = count_sentences(argc, argv);
    sentences = malloc((sentences_nb + 1) * sizeof(int));
    //Store the location of each sentence's beginning in an array, the last element inicates the number of elements from the input.
    get_sentences(argc, argv, sentences, sentences_nb);
    //Requirement states: The number of variables is assumed to be equal to the number of equations.
    variable_instances_nb = sentences_nb;
    variable_instances = malloc((variable_instances_nb)* sizeof(char *));
    get_variable_instances(argv, variable_instances, variable_instances_nb);
    //Sort variables into lexicographical order using strcmp
    sort(variable_instances, variable_instances_nb);
    //Initialise a n x n matrix for variable instances, plus an additional column for constants
    double equations[sentences_nb][sentences_nb + 1];
    for (int i = 0; i < sentences_nb; i++)
        for (int j = 0; j < (sentences_nb + 1); j++)
            equations[i][j] = 0.0;

     
    for (int i = 0; i < variable_instances_nb; i++) 
        printf(" %s ", *(variable_instances + i));
    putchar('\n');
}
int count_sentences(int argc, char **argv) {
    int sentences_nb = 0, i = 0;
    while (--argc && ++i)
        if (!strcmp(argv[i], "The"))
            ++sentences_nb;
    return sentences_nb;
}
void get_sentences(int argc, char **argv, int *sentences, int sentences_nb) {
    int i = 0, j = 0, nb = sentences_nb;
    /* sentences[nb] points to end of sentences in file */
    sentences[nb] = argc;
    while (nb--) {
        sentences[i++] = ++j;
        while (j < argc - 1 && strcmp(argv[j + 1], "The")) {
            ++j;
            if (argv[j][strlen(*(argv + j)) - 1] == ',')
                argv[j][strlen(*(argv + j)) - 1] = '\0';
        }
    /* Replaces full stop at end of sentence by null character. */
        argv[j][strlen(argv[j]) - 1] = '\0';
    }
}
void get_variable_instances(char **argv, char **variable_instances, int variable_instances_nb) {
    int i = 0, n = 1, nb = variable_instances_nb;
    while (nb--) {
        while (argv[n][0] != '_' && (!isalpha(argv[n][0]) || keyword(argv[n], variable_instances)))
            ++n;
        variable_instances[i++] = argv[n++];
    }
}
bool keyword(char* word, char** variable_instances) {
    char *keywords[] = {"The", "result", "of", "multiplying", "by", "plus", "times", "is", "to", 
                        "equal", "product", "and", "the", "adding", "sum", "equals", '\0'};
    for (int i = 0; *(keywords + i); i++)
        if (!strcmp(*(keywords + i), word))
            return true;
    for (int i = 0; *(variable_instances + i); i++)
        if (!strcmp(*(variable_instances +i), word))
            return true; 
    return false;
}
void sort(char** variable_instances, int var_total) {
    bool there_is_change = true;
    if (var_total <= 1)
        return;
    while (there_is_change) {
        there_is_change = false;
        for (int i = 1; i < var_total; i++)
            if (strcmp(*(variable_instances + i - 1), *(variable_instances + i)) > 0) {
                char* addr_holder = *(variable_instances + i - 1);
                *(variable_instances + i - 1) = *(variable_instances + i);
                *(variable_instances + i) = addr_holder;
                there_is_change = true;
            }
    }
}


    
        
        
    
