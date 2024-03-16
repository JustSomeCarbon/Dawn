/*
 * File: err.c
 * Author: Noe Garcia
 * Description: Implementaion of error handling functions used for the Solace
 * compiler during execution. All error functions should be defined in the header
 * file and implemented in this file. All error functions should be available and
 * included in all Solace compiler files.
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
        printf("%s\n", msg);
    }
    // invoke cleanup protocols
    cleanup_ast();
    cleanup_symtable();

    // exit
    exit(-1);
}


/*
 * Cleans the global abstract syntax tree before the program exits
 */
void cleanup_ast()
{
    // TODO
}


/*
 * cleans the global symbol table tree before the program exits
 */
void cleanup_symtable()
{
    // TODO
}