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

/* Sets the value of third argument.
 * Almost totally implemented. */
void get_sentences(int argc, char **argv, int *sentences, int sentences_nb);

/* Sets the value of second argument.
 * Totally implemented. */
void get_variable_instances(char **argv, char **variable_instances, int variable_instances_nb);

/* Returns true if the string provided as argument
 * is one of is, to, of, by, etc.
 * Returns false otherwise.
 * Not implemented */
bool keyword(char *);

int main(int argc, char **argv) {
/* You might declare and use the following variables. */
    
/* Number of sentences in file
 * (equal to number of equations).
 * Code below shows how to determine its value. */
    int sentences_nb;

/* Number of occurrences of variables in file.
 * Code below does not show how to determine its value */
    int variable_instances_nb;

/* The beginning of an array that stores the beginning of each sentence
 * and the end of the last sentence.
 * Code below partially shows how to determine it. */
    int *sentences;

/* The beginning of an array that stores one occurrence of each variable.
 * Code below shows how to determine it. */
    char **variable_instances;

/* Your code might use the previous functions as follows. */

/* To compute and use the value of 'sentences' */
    sentences_nb = count_sentences(argc, argv);
    sentences = malloc((sentences_nb + 1) * sizeof(int));
    get_sentences(argc, argv, sentences, sentences_nb);
    //...
    //free(sentences);

/* To compute and use the value of 'variable_instances' */
    //variable_instances = malloc(variable_instances_nb * sizeof(char *));
    //get_variable_instances(argv, variable_instances, variable_instances_nb);
    //...
    //free(variable_instances);
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
	    /* Here you might want to check if string pointed to
	     * ends in a comma and in case it does,
	     * replace the comma by null character. */
            //...
        }
	/* Replaces full stop at end of sentence by null character. */
        argv[j][strlen(argv[j]) - 1] = '\0';
    }
}
void get_variable_instances(char **argv, char **variable_instances, int variable_instances_nb) {
    int i = 0, n = 1, nb = variable_instances_nb;
    while (nb--) {
        while (argv[n][0] != '_' && (!isalpha(argv[n][0]) || keyword(argv[n])))
            ++n;
        variable_instances[i++] = argv[n++];
    }
}
bool keyword(char* word) {
    char *keywords[] = {"The", "result", "of", "multiplying", "by", "plus", "times", "is", "to", "equal", "product", 
                        "and", "the", "adding", "sum", "equals"};
    for (int i = 0; keywords; i++) {
        if (!strcmp(*(keywords + i), word))
            return true;
    }
    return false;
}


    
        
        
    
