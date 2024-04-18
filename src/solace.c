/*
 * File: solace.c
 * Author: Noe Garcia
 * Date: February, 2023
 * Description: The main Solace compiler file. This file must be compiled
 *  and run before being used. Functionality includes the functional
 *  binding and usage of the grammar and syntax files and functions.
 * 
 * TODO:: Add a debug flag for the parser output.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgram.tab.h"
#include "tree.h"
#include "symtable.h"

// external and globals
extern FILE* yyin;
char* yyfile;
extern int yylex_destroy();
extern struct tree* root;
SymbolTable root_symtable;

// Local Prototypes

void check_extension(char* file);
void step_through_dbFlags(int flag);


// Main Solace Compiler Function
// TODO :: rework input to only take a single source file.

/*
 * Solace compiler main function
 * The main Solace language function that executes the compiler helper functions
 * to parse and process the given source files
 */
int main(int argc, char* argv[])
{
    // Solace compiler
    //  - version 0.0.1 pre-alpha

    int astFlag = 0;

    if (argc == 0) {
        // no file given :: throw error
        printf("No file was given:: file specification required\n");
        printf("Example::  solace file.solace\n\n");
        return 0;
    }

    // loop through arguments
    int filearg = 1;
    // parse each file::
    while (argv[filearg] != NULL) {
        if (strcmp(argv[filearg], "-v") == 0) {
            printf("Solace compiler\n   - version 0.0.1 pre-alpha\n");
            return 0;
        } else if (strcmp(argv[filearg], "-s") == 0) {
            // print the abstract syntax tree
            astFlag = 1;
        } else {
            // compile the source file
            check_extension(argv[filearg]);
            
            // open the given source file
            yyin = fopen(yyfile, "r");
            if (yyin == NULL) {
                // file open error
                printf("Error: unable to open file %s\n\n", yyfile);
                fclose(yyin);
                exit(1);
            }
            // build: AST
            yyparse();

            // build: Symbol Table
            root_symtable = build_symtable(root);
            print_symtable(root_symtable, 0);

            fclose(yyin);
        }
        // move to next source file
        filearg++;
    } // end of while
    // No more source files: end lexer

    step_through_dbFlags(astFlag);

    // Free the root of the AST
    freeTree(root);

    // END OF COMPILATION
    yylex_destroy();
    return 0;
}


/*
 * take the given flag integer and execute flag action
 */
void step_through_dbFlags(int flag)
{
    if (flag) {
        printTree(root, 0);
    }
}


/*
 * Takes a file name and checks whether the extension of the file matches the .solc file extension
 * and populates the yyfile global variable
 * If the given file extension is not a Solace extension, throw an error and exit (-1)
 * If the file does not have an extension, add the .solc extension to the file and populate yyfile
 */
void check_extension(char* file)
{
    char* extension = strrchr(file, '.');
    if (extension != NULL) {
        if (strcmp(extension, ".solc") != 0) {
            printf("Error :: file given is not a solace source file: %s\n\n", file);
            exit(-1); // !- source file error -!
        } else {
            yyfile = (char *) calloc(strlen(file)+6, sizeof(char));
            strncpy(yyfile, file, strlen(file)); // initialize yyfile
            return;
        }
    }
    // add the .solc extension
    yyfile = (char *) calloc(strlen(file)+5, sizeof(char));
    char* end = ".solc";
    char a[strlen(file)+5];
    strncat(a, file, strlen(file));
    strncpy(yyfile, strncat(file, end, 5), strlen(file)+5);
    return;
}