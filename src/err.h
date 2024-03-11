/*
 * File: err.h
 * Author: Noe garcia
 * Date: March 4, 2024
 * Description: header file for the error handling functions for the Solace language.
 *   All error functions should be defined here and implemented in err.c within the
 *   same directory. Some error functions merely take error messages and exit the program.
 *   All functions should initialize emergency clean up of memory when called.
 */

#ifndef ERR_H
#define ERR_H

void throw_err(char* msg, int status);
void check_allocation(void* mem, char* msg);
void cleanup_ast();
void cleanup_symtable();


#endif