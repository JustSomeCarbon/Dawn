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
#include "sgram.tab.h"
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
 * take an abstract syntax tree at a function definition and walk through the entire
 * function populating the symbol table. A new scope is created for the function to
 * contain all defined symbols.
 * nothing is returned
 */
void func_walkthrough(struct tree* ast, SymbolTable current_table)
{
    // break apart into function header and function body
        // if the function name is not main
        SymbolTable new_scope = NULL;
        if (ast->kids[1]->prodrule == NAME) {
            char* name = obtain_name(ast->kids[1]);
            new_scope = enter_new_scope(current_table, name);
            free(name);
        } else if (ast->kids[1]->prodrule == MAINFUNC) {
            new_scope = enter_new_scope(current_symtable, "mainf");
        } else {
            // something went wrong, should have been caught
            throw_err("Function header not defined correctly", 1);
        }
        
        // check if the function header has parameters. if it does, call the
        // function header walkthrough
        if (ast->kids[0]->kids[2] != NULL) {
            // pass ast at ParamList
            func_header_walkthrough(ast->kids[0]->kids[2]->kids[0], current_table);
        }
        // check if the function body has expression, if it does, call the
        // function body walkthrough
        if (ast->kids[1] != NULL) {
            // pass ast at FunctionBodyDecls
            func_body_walkthrough(ast->kids[1]->kids[0]);
        }
}


/*
 * walks through the given function header abstract syntax tree and populates the
 * symbol table with the parameters passed.
 * nothing is returned
 */
void func_header_walkthrough(struct tree* ast, SymbolTable current_table)
{
    if (ast->nkids == 2) {
            // Populate symtable with parameter
            char* name = obtain_name(ast->kids[1]->kids[0]);
            int index = insert_symbol_entry(current_symtable, name);
            free(name);

            func_header_walkthrough(ast->kids[1], current_symtable);
        }
        //
        func_header_walkthrough(ast->kids[0], current_symtable);
}


/*
 * takes the abstract syntax tree and walk through the function body expressions.
 * This function expects the ast given to be the function body declarations.
 * nothing is returned
 */
void func_body_walkthrough(struct tree* ast, SymbolTable current_symtable)
{
    //
}