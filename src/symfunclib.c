/*
 * File: sympop.c
 * Author: Noe Garcia
 * Date: February 9, 2024
 * Description: Contains all the logic and implementations for the helper
 * functions for the symbol table population functionality of the Solace compiler.
 * These functions are made to work with the symtable.c file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "symtable.h"
#include "err.h"

/*
 * Expects to take an ast of a Name rule. Returns the name of the ast rule
 * as a malloced char*. Character pointers returned are allocated on the heap and must be freed
 * ex: an ast containing: `Name COLON IDENTIFIER(bar)` with the Name being an `IDENTIFIER(foo)`
 * would return `"foo:bar"`
 */
char* obtain_name(struct tree* ast)
{
    char* name = NULL;
    if (ast->nkids == 3) {
        // concatenate the return value of calling obtain name with the next
        // Name rule bellow
        char* front = obtain_name(ast->kids[0]);
        name = (char*)malloc(strlen(front) + strlen(ast->kids[2]->leaf->text) + 2);
        check_allocation(name, "Error in Name memory allocation during symbol parse...");
        strcpy(name, front);
        if (strcmp(ast->kids[1]->leaf->text, ":") == 0) {
            strcat(name, ":");
        } else if (strcmp(ast->kids[1]->leaf->text, ".") == 0) {
            strcat(name, ".");
        } else {
            // throw an error, something went wrong
            char buf[100];
            snprintf(buf, sizeof(buf), "Given character %s not supported in name\n", ast->kids[1]);
            throw_err(buf, 1);
        }

        strcat(name, ast->kids[2]->leaf->text);
        name[strlen(name)-1] = "\0";

        free(front);
    } else if (ast->nkids == 2) {
        // throw an error, this is an access to a field of an array
        throw_err("Field value access not allowed in name definition\n", 1);
    } else {
        // copy the string name to the return value
        // and return it
        name = (char*)malloc(strlen(ast->kids[0]->leaf->text) + 1);
        check_allocation(name, "Error in Name memory allocation during symbol parse...");
        strcpy(name, ast->kids[0]->leaf->text);
    }
    return name;
}

/*
 *
 */
SymbolEntry is_struct_decl()
{
    //
}

/*
 *
 */
SymbolEntry is_function_header()
{
    //
}

/*
 *
 */
SymbolEntry is_variable_decl()
{
    //
}