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


/* -- local function prototypes -- */
void func_param_walkthrough(struct tree* ast, SymbolTable current_table);
void func_body_walkthrough(struct tree* ast, SymbolTable current_table);
void parse_body_decl(struct tree* ast, SymbolTable current_symtable);
void parse_expression(struct tree* ast, SymbolTable current_symtable);
void walk_to_postfix(struct tree* ast);
void parse_postfix(struct tree* ast, SymbolTable current_symtable);
void parse_pattern_block(struct tree* ast, SymbolTable current_symtable);

int check_if_expr_assignment(struct tree* ast);


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
            snprintf(buf, sizeof(buf), "Given character %s not supported in name\n", ast->kids[1]->leaf->text);
            throw_err(buf, 1);
        }

        strcat(name, ast->kids[2]->leaf->text);
        name[strlen(name)-1] = '\0';

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
        if (ast->kids[0]->kids[0]->prodrule == NAME) {
            char* name = obtain_name(ast->kids[1]);
            new_scope = enter_new_scope(current_table, name);
            free(name);
        } else if (ast->kids[0]->kids[0]->prodrule == MAINFUNC) {
            new_scope = enter_new_scope(current_symtable, "mainf");
        } else {
            // something went wrong, function does not have a name
            throw_err("Function header not defined correctly", 1);
        }
        
        // check if the function header has parameters. if it does, call the
        // function header walkthrough
        if (ast->kids[0]->kids[2] != NULL) {
            // pass ast at ParameterList
            func_param_walkthrough(ast->kids[0]->kids[2]->kids[0], new_scope);
        }
        
        // check if the function body has expression(s), if it does, call the
        // function body walkthrough
        if (ast->kids[1] != NULL) {
            // pass ast at FunctionBodyDecls
            func_body_walkthrough(ast->kids[1]->kids[0], new_scope);
        }
}


/*
 * walks through the given function header abstract syntax tree and populates the
 * symbol table with the parameters passed.
 * nothing is returned
 */
void func_param_walkthrough(struct tree* ast, SymbolTable current_table)
{
    if (ast->nkids == 2) {
            // Populate symtable with parameter
            char* name = obtain_name(ast->kids[1]->kids[0]);
            int index = insert_symbol_entry(current_symtable, name);
            free(name);

            func_param_walkthrough(ast->kids[0], current_symtable);
        }
        func_param_walkthrough(ast->kids[0], current_symtable);
}


/*
 * takes the abstract syntax tree and walk through the function body declarations.
 * This function expects the ast given to be the function body declarations.
 * nothing is returned
 */
void func_body_walkthrough(struct tree* ast, SymbolTable current_symtable)
{
    if (ast->nkids == 2) {
        parse_body_decl(ast->kids[1], current_symtable);
        func_body_walkthrough(ast->kids[0], current_symtable);
    }
    parse_body_decl(ast->kids[0], current_symtable);
}


/*
 * takes an individual function body declaration and populates the symbol table
 * with the appropriate symbols that are defined within the given expression.
 * if the experssion is instead a pattern block, this function passes control to
 * the pattern block parsing function.
 * nothing is returned
 */
void parse_body_decl(struct tree* ast, SymbolTable current_symtable)
{
    if (ast->nkids == 2) {
        // return expression: RETURN expr
        // return type magic ...
        parse_expression(ast->kids[1], current_symtable);
    } else {
        if (ast->kids[0]->prodrule == PATTERN_BLOCK) {
            // pass to pattern block parser
            SymbolTable new_scope = enter_new_scope(current_symtable, "pattern_block");
            parse_pattern_block(ast->kids[0], new_scope);
        } else {
            // parse the expression
            parse_expression(ast->kids[0], current_symtable);
        }
    }
}


/*
 * takes an expression found in a function body and parses the symbols defined.
 * the symbols defined in the expression are added to the symbol table of the 
 * nothing is returned
 */
void parse_expression(struct tree* ast, SymbolTable current_symtable)
{
    if (ast->kids[0]->prodrule == VAR_ASSIGNMENT) {
        // variable assignment operation
        if (ast->kids[0]->kids[0]->nkids == 2) {
            char* name = obtain_name(ast->kids[0]->kids[0]->kids[0]);
            int index = insert_symbol_entry(current_symtable, name);
            free(name);
        }
        // if not, drop val
    } else {
        // walk to postfix expression
        struct tree* tmp_ast = ast;
        walk_to_postfix(tmp_ast);
        parse_postfix(tmp_ast, current_symtable);
    }
}


/*
 * takes an ast at an expression and walks to the postfix expression for further processing.
 * moves the given ast pointer down the ast to postfix.
 * nothing is returned
 */
void walk_to_postfix(struct tree* ast)
{
    while (ast->prodrule != POST_FIX_EXPR) {
        if (ast->nkids == 3) {
            ast = ast->kids[2];
        } else if (ast->nkids == 2) {
            ast = ast->kids[1];
        } else {
            ast = ast->kids[0];
        }
    }
}


/*
 * takes a postfix expression and ensures validity of the program.
 */
void parse_postfix(struct tree* ast, SymbolTable current_symtable)
{
    // if expression pass to expression walkthrough
    if (ast->kids[0]->prodrule == PRIMARY) {
        if (ast->kids[0]->kids[0]->prodrule == EXPR) {
            parse_expression(ast->kids[0]->kids[0], current_symtable);
            return;
        }
    }
}


/*
 * takes a pattern block ast and parses the symbols. symbols are placed in the
 * given symbol table.
 * nothing is returned
 */
void parse_pattern_block(struct tree* ast, SymbolTable current_symtable)
{
    if (ast->nkids == 2) {
        int last_kid = ast->kids[1]->nkids - 1;
        func_body_walkthrough(ast->kids[1]->kids[last_kid], current_symtable);
        parse_pattern_block(ast->kids[0], current_symtable);
    } else {
        int last_kid = ast->kids[0]->nkids - 1;
        func_body_walkthrough(ast->kids[0]->kids[last_kid], current_symtable);
    }
}


/* -- For future error checking -- */

/*
 * check if the given expression is an assignment. if it is,
 * return 1 (true), otherwise, return 0 (false)
 */
int check_if_expr_assignment(struct tree* ast)
{
    return 0;
}