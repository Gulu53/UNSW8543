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

typedef struct {int start; int end;}Skip;

int count_sentences(int argc, char **argv);
void get_sentences(int argc, char **argv, int *sentences, int sentences_nb);
void get_variable_instances(char **argv, char **variable_instances, int variable_instances_nb);
bool keyword(char *, char**);
void sort(char**, int);
void process_sentences(char **argv, int *sentences, int nth_equation, int sentence_start, int sentences_nb, double **equation, char **variable_instances);
bool isnumeric(double *number, char *input);
bool isimportant(char *keyword, int *category); 
bool isvariable(char *keyword, char **variable_instances, int *index, int sentences_nb); 
double times (char **argv, int *pt_i, int *nth_colume, char **variable_instances, int sentences_nb); 
double product_multiplying(char **argv, int *pt_i, int *nth_colume, char **variable_instances, char *end_word, int sentences_nb);

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
        process_sentences(argv, i, *sentences, sentences_nb, equations, variable_instances);

    for (int i = 0; i < variable_instances_nb; i++) 
        printf(" %s ", *(variable_instances + i));
    putchar('\n');
}
void process_sentences(char **argv, int *sentences, int nth_equation, int sentence_start, int sentences_nb, double **equation, char **variable_instances) {
    int category = 0, index = 0, *pt_category = &category, *pt_index = &index, list_size = 1;
    double number = 0, *pt_number = &number;
    bool inverse = false;
    Skip *pt_skip_list = malloc((list_size) * sizeof(Skip));
    //Calculate all the mutiplications and records the start and end of each multiplication statements' locations 
    for (int i = sentence_start; *(argv + i); i++) {
        if (isimportant(*(argv + i), pt_category)) {
            if (category >= 3) {
                inverse = true;
                continue;
            }
            int *pt_i = &i, nth_colume = sentences_nb, *pt_nth_colume = &nth_colume;
            double result = 0;
            list_size++;
            pt_skip_list = realloc(pt_skip_list, (list_size * sizeof(Skip)));
            pt_skip_list[list_size - 2].start = (category == 2) ? (i - 1) : i;
            if (category == 2) 
                result = times(argv, pt_i, pt_nth_colume, variable_instances, sentences_nb); 
            else {
                char *end_word = (category) ? "by" : "and";
                result = product_multiplying(argv, pt_i, pt_nth_colume, variable_instances, end_word, sentences_nb);  
            }
            pt_skip_list[list_size - 2].end = i;
            equation[nth_equation][nth_colume] += (inverse) ? (-1 * result) : result; 
        }
    }
    //Calculate all the addition, skip the multiplication statements
    int list_read_pt = 0;
    inverse = false;
    for (int i = sentence_start; *(argv + i); i++) {
        //If this is the start of a multiplication stament, skip it.
        if (list_read_pt <= list_size) {
            if (pt_skip_list[list_read_pt].start == i) {
                i = pt_skip_list[list_read_pt].end;
                list_read_pt++; 
                continue;
            } 
        }
        //If this keyword is equal or equals
        if (isimportant(*(argv + i), pt_category)) { 
            inverse = (category >= 3) ? true : false;
            continue;
        }
        //If this keyword is the varaible instance
        if (isvariable(*(argv + i), variable_instances, pt_index, sentences_nb)) {
            equation[nth_equation][index] = (inverse) ? (equation[nth_equation][index] - 1) : (equation[nth_equation][index] + 1);
            continue;
        }
        //If this keyword is numeric value
        if (isnumeric(pt_number, *(argv + i))) {
            equation[nth_equation][sentences_nb] = (inverse) ? (equation[nth_equation][sentences_nb] - number) : (equation[nth_equation][sentences_nb] + number);
            continue;
        }
    }
    free(pt_skip_list);
    equation[nth_equation][sentences_nb] = equation[nth_equation][sentences_nb] * -1;
    for (int i = 0; variable_instances[i]; i++)
        printf("%f%s", equation[nth_equation][i], variable_instances[i]);
    printf(" = %f\n", equation[nth_equation][sentences_nb]);
}
double product_multiplying(char **argv, int *pt_i, int *nth_colume, char **variable_instances, char *end_word, int sentences_nb) {
    int i = *pt_i;
    double ans = 1, number = 0, *pt_number = &number;
    while (!(isnumeric(pt_number, argv[i]) || isvariable(argv[i], variable_instances, nth_colume, sentences_nb))) 
        i++;
    while (strcmp(argv[i],end_word)) {
        if (isnumeric(pt_number, argv[i])) 
            ans *= number;
        else 
            isvariable(argv[i], variable_instances, nth_colume, sentences_nb);
        i++;
    }
    if (isnumeric(pt_number, argv[i + 1])) 
        ans *= number;
    else 
        isvariable(argv[i + 1], variable_instances, nth_colume, sentences_nb);
    *pt_i = i + 1;
    return ans;
}
double times(char **argv, int *pt_i, int *nth_colume, char **variable_instances, int sentences_nb) {
    int i = *pt_i;
    double ans = 1, number = 0, *pt_number = &number;
    for (int j = -1; j <= 1; j += 2) { 
        if (isnumeric(pt_number, argv[i + j])) 
            ans *= number;
        else 
            isvariable(argv[i + j], variable_instances, nth_colume, sentences_nb);
    }
    *pt_i = *pt_i + 1;
    return ans;
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
bool isvariable(char *keyword, char **variable_instances, int *index, int sentences_nb) {
    for (int i = 0; i < sentences_nb; i++) {
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
    *number = 0;
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
            if (decimal_point)
                *number = *number + (double)(*(input + i) - '0') * pow(10.0, power);
            else 
                *number = *number * pow(10, power) + (double)(*(input + i) - '0');
            if (decimal_point)
                --power;
            else
                power = 1;
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


    
        
        
    
