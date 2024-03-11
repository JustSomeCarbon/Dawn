/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "err.h"

/*
 * Logs the given message to the console before crashing the program with
 * the given status code
 */
void throw_err(char* msg, int status)
{
    printf("%s\n", msg);
    exit(status);
}

/*
 * checks to ensure that the given memory allocation is not NULL (something
 * went wrong). If the memory allocation is NULL print the error message given
 * to the console, and invoke cleanup protocols before crashing the program
 */
void check_allocation(void* mem, char* msg) {
    if (mem == NULL) {
        printf("%s", msg);
    }
    // invoke cleanup protocols

    // exit
    exit(-1);
}