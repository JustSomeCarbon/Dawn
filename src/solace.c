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
    int filearg = 0;
    // parse each file::
    while (argv[filearg] != NULL) {
        if (strcmp(argv[filearg], "-v") == 0) {
            printf("Solace compiler\n   - version 0.0.1 pre-alpha\n");
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

                tltail->t->category = tokcat;
                tltail->t->text = strdup(yytext);
                tltail->t->lineno = yylineno;
                tltail->t->filename = argv[filearg];

                switch (tokcat)
                {
                case LITERALINT:
                    tltail->t->ival = atoi(yytext);
                    break;
                case LITERALHEX:
                    tltail->t->ival = strtol(yytext+2, NULL, 16);
                    break;
                case LITERALFLOAT:
                    tltail->t->dval = strtod(yytext, NULL);
                    break;
                case LITERALSTRING:
                    tltail->t->sval = calloc(1, yyleng); // may be too long
                    int walk = 1;
                    while (*(yytext+walk) != '\0') {
                        if (*(yytext+walk) == '\\') {
                            // handle escape characters
                            walk++;
                            switch (*(tltail->t->text+walk))
                            {
                            case 'n':
                                *(tltail->t->sval+walk) = 0x0a;
                                break;
                            case 't':
                                *(tltail->t->sval+walk) = 0x09;
                                break;
                            case '\\':
                                *(tltail->t->sval+walk) = 0x5c;
                                break;
                            case '\'':
                                *(tltail->t->sval+walk) = 0x27;
                                break;
                            case '\"':
                                *(tltail->t->sval+walk) = 0x22;
                                break;
                            default:
                                printf("Error: Unsupported escape character %s at line %d in file %s\n\n", yytext, yylineno, argv[filearg]);
                                fclose(yyin);
                                exit(1);
                                break;
                            }
                        } else {
                            *(tltail->t->sval+walk) = *(yytext+walk);
                        }
                        walk++;
                    }
                    *(tltail->t->sval+walk) = '\0';
                    break;
                case LITERALCHAR:
                    int walk = 1; // skip the single quote character
                    if (*(yytext+walk) == '\\') {
                        // handle escape characters
                        walk++;
                        switch (*(tltail->t->text+walk))
                        {
                        case 'n':
                            tltail->t->ival = 0x0a;
                            break;
                        case 't':
                            tltail->t->ival = 0x09;
                            break;
                        case '\\':
                            tltail->t->ival = 0x5c;
                            break;
                        case '\'':
                            tltail->t->ival = 0x27;
                            break;
                        case '\"':
                            tltail->t->ival = 0x22;
                            break;
                        default:
                            printf("Error: unsupported escape character %s at line %d in file %s\n\n", yytext, yylineno, argv[filearg]);
                            break;
                        }
                    } else {
                        tltail->t->ival = *(yytext+walk);
                    }
                    break;
                default:
                    break;
                }

                tltail->next = allocateToken();
                if (tltail->next == NULL) {
                    printf("Error: unable to allocate memory space for lexer token");
                    fclose(yyin);
                    exit(1);
                }
                tltail = tltail->next;
            } // end of lexer while
            fclose(yyin);

            // print the token list
            printTokenList(tl);
        }
        // move to next source file
        filearg++;
    } // No more source files: end lexer

    // END OF COMPILATION
    yylex_destroy();
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