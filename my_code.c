/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Processes 6 command line arguments, modifying them and         *
 *              shuffling them around.                                         *
 *                                                                             *
 * Written by *** for COMP9021                                                 *
 *                                                                             *
 * Other source files, if any, one per line, starting on the next line:        *
 *       quiz7.c                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ctype.h>
#include <string.h>

#define SIZE 6

void process_command_line_arguments(char **argv, int argc) {
 
    char end_mark = '\0', *order[SIZE];
    int sequence[] = { 5, 6, 1, 3, 2 ,4 };
    
    for (int i = 0; i < SIZE; i++) 
        order[i] = *(argv + sequence[i]);     
    
    for (int i = 1; i < argc; i++) {
        signed int j = 0, write_pos = -1;
        while (*(*(argv + i) + j)) { 
            switch (i) {
                case 1:
                    if (((*(*(argv + i) + j)) >= 'a') && ((*(*(argv + i) + j)) < 'n')) { 
                        write_pos++;
                        if (write_pos != j)
                            (*(*(argv + i) + write_pos)) = (*(*(argv + i) + j));  
                    }        
                    break;
                case 2:
                    if (((*(*(argv + i) + j)) > 'm') && ((*(*(argv + i) + j)) <= 'z')) { 
                        write_pos++;
                        if (write_pos != j)
                            (*(*(argv + i) + write_pos)) = (*(*(argv + i) + j));  
                    }        
                    break;
                case 3:
                    if (((*(*(argv + i) + j)) >= 'A') && ((*(*(argv + i) + j)) < 'N')) { 
                        write_pos++;
                        if (write_pos != j)
                            (*(*(argv + i) + write_pos)) = (*(*(argv + i) + j));  
                    }        
                    break;
                case 4:
                    if (((*(*(argv + i) + j)) > 'M') && ((*(*(argv + i) + j)) <= 'Z')) { 
                        write_pos++;
                        if (write_pos != j)
                            (*(*(argv + i) + write_pos)) = (*(*(argv + i) + j));  
                    }        
                    break;
                case 5:
                    if (isdigit(*(*(argv + i) + j))) { 
                        write_pos++;
                        if (write_pos != j)
                            (*(*(argv + i) + write_pos)) = (*(*(argv + i) + j));  
                    }
                    break;    
                case 6: 
                    if (ispunct(*(*(argv + i) + j))) { 
                        write_pos++;
                        if (write_pos != j)
                            (*(*(argv + i) + write_pos)) = (*(*(argv + i) + j));  
                    }
                    break;
            }
            j++;
        }
        (*(*(argv + i) + write_pos + 1)) = end_mark; 
    }

    for (int i = 0; i < SIZE; i++)
        argv[i + 1] = order[i];
}
