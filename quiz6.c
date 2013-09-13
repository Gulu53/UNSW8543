/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Randomly fills an array of size 10x10 with 0s and 1s, and      *
 *              outputs the number of shortest paths linking two points        *
 *              with nothing but 0s or nothing but 1s on the path              *
 *              (we consider the theoretically shortest paths, so there        *
 *              can exist a path but no shortest path in this sense).          *
 *                                                                             *
 * Written by Eric Martin for COMP9021                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM 10

int grid[DIM][DIM];

void display_grid(void);
int number_of_shortest_paths(int, int, int, int);

int main(int argc, char **argv) {
    /* We run the program with six command line arguments. */
    if (argc != 7) {
        printf("Please provide 5 command line arguments.\n");
        return EXIT_FAILURE;
    }
    /* The first one will determine our probability distribution;
     * it is meant to be strictly positive. */
    int nb_of_possible_outcomes = strtol(argv[1], NULL, 10);
    /* We use the second command line argument, meant to be a nonnegative integer,
     * as a seed for the random number generator. */
    srand(strtoul(argv[2], NULL, 10));
    /* We fill the grid with randomly generated 0s and 1s,
     * with for every cell, a probability of 1/nb_of_possible_outcomes to generate a 0. */
    for (int i = 0; i < DIM; ++i)
        for (int j = 0; j < DIM; ++j)
            grid[i][j] = rand() % nb_of_possible_outcomes ? 1 : 0;
    puts("Here is the grid that has been generated:\n");
    display_grid();
    int x1 = strtol(argv[3], NULL, 10);
    int y1 = strtol(argv[4], NULL, 10);
    int x2 = strtol(argv[5], NULL, 10);
    int y2 = strtol(argv[6], NULL, 10);
    if (x1 < 0 || y1 < 0 || x1 >= DIM || y1 >= DIM) {
        puts("The third and fourth command line arguments cannot be the x and y coordinates\n"
               "  of a point in the grid; run the program again!");
        return EXIT_FAILURE;
    }
    if (x2 < 0 || y2 < 0 || x2 >= DIM || y2 >= DIM) {
        puts("The fifth and sixth command line arguments cannot be the x and y coordinates\n"
               "  of a point in the grid; run the program again!");
        return EXIT_FAILURE;
    }
    printf("Number of shortest paths from point (%d, %d) to point (%d, %d): %d\n",
           x1, y1, x2, y2, number_of_shortest_paths(x1, y1, x2, y2));
    return EXIT_SUCCESS;
}

void display_grid(void) {
    for (int i = 0; i < DIM; ++i) {
        printf("    ");
        for (int j = 0; j < DIM; ++j)
            grid[i][j] ? printf(" 1") : printf(" 0");
        putchar('\n');
    }
    putchar('\n');
}

/* A simple recursive function... */
int number_of_shortest_paths(int x1, int y1, int x2, int y2) {
    int path_num = 0;    
    if ((grid[x1][y1] ==  grid[x2][y2])) { 
        if ((x1 == x2) && (y1 == y2)) 
            return path_num += 1;
    }
    else {
        return 0;
    }

    if (x1 != x2) 
        path_num += x1 > x2 ? number_of_shortest_paths(x1 - 1, y1, x2, y2) : number_of_shortest_paths(x1 + 1, y1, x2, y2);
    if (y1 != y2) 
        path_num += y1 > y2 ? number_of_shortest_paths(x1, y1 - 1, x2, y2) : number_of_shortest_paths(x1, y1 + 1, x2, y2);    
    return path_num;
}
