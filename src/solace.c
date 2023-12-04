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

// external and globals
extern FILE* yyin;
char* yyfile;
extern int yylex_destroy();
extern struct tree* root;

// Local Prototypes

void check_extension(char* file);


// Main Solace Compiler Function

int main(int argc, char* argv[])
{
    // Solace compiler
    //  - version 0.0.1 pre-alpha

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
        } else {
            // compile the source file
            check_extension(argv[filearg]);
            
            // loop through parser: TEST
            yyin = fopen(yyfile, "r");
            if (yyin == NULL) {
                // file open error
                printf("Error: unable to open file %s\n\n", yyfile);
                fclose(yyin);
                exit(1);
            }
            // Parse the source file given
            printf("File parse returns:: %d\n\n", yyparse());
            printTree(root, 0);

            printf("Freeing source file...");
            freeTree(root);

            // close the file
            fclose(yyin);
        }
        // move to next source file
        filearg++;
    } // No more source files: end lexer

    // END OF COMPILATION
    yylex_destroy();
    return 0;
}


/*
 * check_extension: takes a file name and checks whether the extension of the file matches the .solc file extension.
 *  Populates the yyfile global for further compilation.
 *  If it does not throw an error and exits (-1).
 *  If the file does not have an extension, add the .solc extension to the file and populate yyfile.
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