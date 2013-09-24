/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:                                                                *
 *                                                                             *
 * Written by Kim for COMP9021                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MIN_WIDTH 2
#define MAX_WIDTH 50
#define MIN_HEIGHT 2
#define MAX_HEIGHT 50
#define N 0
#define NE 1
#define E 2
#define SE 3
#define S 4
#define SW 5
#define W 6
#define NW 7
#define GET_ID(X) (X & 0b0000111111111111)
#define GET_DIRECTION(X) ((X >> 12) & 0b0111)
#define PAINT(DIRECTION, ID) ((DIRECTION << 12) + ID)

signed short int input_data[MAX_WIDTH][MAX_HEIGHT];

bool get_input(void);
bool get_polygon(int, int, int, int, int);
int get_direction(int, int, int);
void print_matrix(void); 

int main(int argc, char **argv) {
    if (argc > 2 || argc == 2 && strcmp(argv[1], "print")) {
        printf("I expect no command line argument or \"print\" as unique command line argument.\n");
        return EXIT_FAILURE;
    }
    if (!get_input()) {
        printf("Incorrect input.\n");
        return EXIT_FAILURE;
    }
    if (argc == 1) {
        /* Insert your code for the case a.out is run with no command line argument */
    }
    else {
        /* Insert your code for the case a.out is run with print as command line argument */
    }
    return EXIT_SUCCESS;
}

bool get_input(void) {
    int input_char = 0, expected_width = 0, x_write_loc = 0, y_write_loc = 0;
    for (int y = 0; y < MAX_HEIGHT; y++)
        for (int x = 0; x < MAX_WIDTH; x++)
            input_data[y][x] = -1;
    while ((input_char = getchar()) != EOF) {
        if (input_char == '\n') { 
            if (!(y_write_loc))
                expected_width = x_write_loc;
            else
                if (x_write_loc != expected_width)
                    return false;
            y_write_loc++; 
            x_write_loc = 0;
            continue;
        } 
        else if ((input_char - '0' != 0) && (input_char - '0' != 1)) 
            return false;
        else if (isspace(input_char))
            continue;
        else {        
            input_data[y_write_loc][x_write_loc] = (signed short int)(input_char - '0'); 
            x_write_loc++;
        }
        if ((x_write_loc > MAX_WIDTH) || (y_write_loc > MAX_HEIGHT))
            return false;
    }
    int polygon_id = 2; 
    for (int y = 0; y < MAX_HEIGHT; y++)
        for (int x = 0; x < MAX_WIDTH; x++) {
            if (input_data[y][x] > 0)
                break;
            if (input_data[y][x] == 1) {
                bool ret = get_polygon(x, y, x, y, polygon_id);
                if (ret)
                    polygon_id++;            
                else
                    return false;
            }
        }
    return true;    
}

bool get_polygon(int org_x, int org_y, int current_x, int current_y, int polygon_id) {
    if ((GET_ID(input_data[y][x]) != 1) && (org_x == current_x) && (org_y == current_y)) 
        return true;
}

int get_direction(int current_x, int current_y, int polygon_id) {

}

void print_matrix(void) { 
    for (int y = 0; y < MAX_HEIGHT; y++) {
        for (int x = 0; x < MAX_WIDTH; x++) 
            printf("%d", input_data[y][x]);
        putchar('\n');
    } 
}

