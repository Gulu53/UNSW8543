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
#include <math.h>

typedef struct {int start; int end}Skip;

int count_sentences(int argc, char **argv);
void get_sentences(int argc, char **argv, int *sentences, int sentences_nb);
void get_variable_instances(char **argv, char **variable_instances, int variable_instances_nb);
bool keyword(char *, char**);
void sort(char**, int);
void process_sentences(char **argv, int equation_nb, int sentence_start, int matrix_size, double **equation, char **variable_instances);
bool isnumeric(double *number, char *input);
bool isimportant(char *keyword, int *category); 
bool isvariable(char *keyword, char **variable_instances, int *index); 
void multiplication (char **argv, int *pt_i, int category, double **equation, int equation_nb, char **variable_instances); 

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
    double **equations = malloc((sentences_nb) * sizeof(double*));
    for (int i = 0; i < sentences_nb; i++)
        *(equations + i) = malloc((sentences_nb + 1) * sizeof(double)); 
    for (int i = 0; i < sentences_nb; i++)
        for (int j = 0; j < (sentences_nb + 1); j++)
            equations[i][j] = 0.0;
    for (int i = 0; i < sentences_nb; i++)
        process_sentences(argv, i, *(sentences + i), sentences_nb, equations, variable_instances);

    for (int i = 0; i < variable_instances_nb; i++) 
        printf(" %s ", *(variable_instances + i));
    putchar('\n');
}
void process_sentences(char **argv, int equation_nb, int sentence_start, int matrix_size, double **equation, char **variable_instances) {
    int category = 0, index = 0, *pt_category = &category, *pt_index = &index, list_size = 1;
    double number = 0, *pt_number = &number;
    bool inverse = false;
    Skip *pt_skip_list = malloc((list_size) * sizeof(Skip));
    for (int i = sentence_start; *(argv + i); i++) {
        //If this keyword is going to execute mutiplication
        if (isimportant(*(argv + i), pt_category)) {
            if (category >= 3) {
                inverse = true;
                continue;
            }
            else {
                int *pt_i = &i;
                list_size++;
                 
                multiplication (argv, pt_i, category, equation, equation_nb, variable_instances); 
            }
        }
    }
    for (int i = sentence_start; *(argv + i); i++) {
        //If this keyword is the varaible instance
        if (isvariable(*(argv + i), variable_instances, pt_index)) {
            equation[equation_nb][index] = (inverse) ? (equation[equation_nb][index] - 1) : (equation[equation_nb][index] + 1);
            continue;
        }
        //If this keyword is numeric value
        if (isnumeric(pt_number, *(argv + i))) {
            equation[equation_nb][matrix_size] = (inverse) ? (equation[equation_nb][matrix_size] - number) : (equation[equation_nb][matrix_size] + number);
            number = 0;
            continue;
        }
    }
    equation[equation_nb][matrix_size] = equation[equation_nb][matrix_size] * -1;
    printf("The answer is %f\n", equation[equation_nb][matrix_size]);
}
void multiplication (char **argv, int *pt_i, int category, double **equation, int equation_nb, char **variable_instances) {
    char  *words[] = {"and", "by", '\0'}; 
    int i = *pt_i;
    double ans = 1, number = 0, *pt_number = &number;
    //If the keyword is times
    if (category == 2) {
        if (isnumeric(pt_number, argv[i - 1]))

    }
}
bool isimportant(char *keyword, int *category) {   
    //Only taking the multiplication operations as important
    char *words[] = {"product", "multiplying", "times", "equals", "equal", '\0'};
    for (int i = 0; *(words + i); i++) {
        if (!(strcmp(keyword, *(words + i)))) {
            *category = i;
            return true;
        }
    } 
    return false;
}
bool isvariable(char *keyword, char **variable_instances, int *index) {
    for (int i = 0; *(variable_instances + i); i++) {
        if (!(strcmp(keyword, *(variable_instances + i)))) {
            *index = i;
            return true;
        }
    } 
    return false;
}
bool isnumeric(double *number, char *input) {
    //Return false if there are alphabets presents
    for (int i = 0; *(input + i); i++) {
        if (isalpha(*(input + i)))
            return false;
        if ((*(input + i) == '-') && (i != 0))
            return false;
    }
    bool neg = (*(input) == '-') ? true : false, decimal_point = false;
    double power = 0, ans = 0;
    for (int i = 0; *(input + i); i++) {
        if ((neg) && (i == 0))
            continue;  
        if (*(input + i) == '.') {
            if (!(decimal_point)) {
                decimal_point = true;
                power = -1;
                continue;
            }
            else
                return false;
        }
        if (isdigit(*(input + i))) {
            *number = *number + (double)(*(input + i) - '0') * pow(10.0, power);
            printf("The sum is %f\n", *number);
            power = (decimal_point) ? --power : ++power;
        }
    }
    *number = (neg) ? (*number * -1) : *number;
    return true;
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


    
        
        
    
