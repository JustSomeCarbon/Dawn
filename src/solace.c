/*
 * File: solace.c
 * Author: Noe Garcia
 * Date: February, 2023
 * Description: The main Solace compiler file. This file must be compiled
 *  and run before being used. Functionality includes the functional
 *  binding and usage of the grammar and syntax files and functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solace.h"
#include "tree.h"

// external and globals
extern FILE* yyin;
char* yyfile;

// prototypes
void check_extension(char* file);


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
    int i = 0;
    while (argv[i] != NULL) {
        if (strcmp(argv[i], "-v") == 0) {
            printf("Solace compiler\n   - version 0.0.1 pre-alpha\n");
        } else {
            // compile the source file
            check_extension(argv[i]);
            
            // loop through parser: TEST
            yyin = fopen(yyfile, "r");
            if (yyin == NULL) {
                // file open error
                printf("Error: unable to open file %s\n\n", yyfile);
                fclose(yyin);
                exit(1);
            }

            int tokcat = 1;
            struct tokenlist* tl = NULL;
            struct tokenlist* tltail = NULL;

            tl = calloc(1, (sizeof(struct tokenlist)));
            tltail = tl;
            if (tl == NULL) {
                printf("Error: unable to allocate memory for lexical tokenizer\n\n");
                fclose(yyin);
                exit(1);
            }

            // create a new token
            tltail->t = calloc(1, sizeof(struct token));
            tltail->next = NULL;
            if (tltail->t == NULL) {
                printf("Error: token memory allocation failure\n\n");
                exit(1);
            }

            int toklen = 0;
            // lexer process ->
            while (tokcat) {
                toklen++;
                tokcat = yylex();
            }

        }
        i++;
    }

    return 0;
}


void check_extension(char* file)
{
    char* extension = strrchr(file, '.');
    if (extension != NULL) {
        if (strcmp(extension, "solace") != 0) {
            printf("Error :: file given is not a solace source file!\n");
            exit(-1); // !- source file access error -!
        } else {
            yyfile = (char *) calloc(strlen(file)+8, sizeof(char));
            strncpy(yyfile, file, strlen(file)); // initialize yyfile
            return;
        }
    }
    // add the .solace extension
    yyfile = (char *) calloc(strlen(file)+7, sizeof(char));
    char* end = ".solace";
    char a[strlen(file)+7];
    strncat(a, file, strlen(file));
    strncpy(yyfile, strncat(file, end, 7), strlen(file)+7);
    return;
}