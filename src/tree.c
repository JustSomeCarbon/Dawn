/*
 * file: tree.c
 * author: Noe Garcia
 * description: Contains all functions regarding the syntax tree construction
 *  and traversal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "tree.h"
#include "sgram.tab.h"

/*
 * Initialize and allocate a new token structure based on the defined integer code value.
 * When complete, returns the code as an integer.
 */
int allocToken(int code)
{
    // initialize the memory for a new tree-with-leaf node
    yylval.treeptr = (struct tree*) calloc(1, sizeof(struct tree));
    if (!yylval.treeptr) {
        printf("Error: memory allocation for tree node failed.\n\n");
        exit(1);
    }
    yylval.treeptr->prodrule = code;
    yylval.treeptr->symbolname = NULL;
    yylval.treeptr->nkids = 0;

    // allocate memory for a new token
    yylval.treeptr->leaf = (struct token*)calloc(1, sizeof(struct token));
    if (!yylval.treeptr->leaf) {
        printf("Error: mamory allocation for token node failed.\n\n");
        exit(1);
    }

    // populate the new token
    yylval.treeptr->leaf->category = code;
    yylval.treeptr->leaf->text = strdup(yytext);
    yylval.treeptr->leaf->lineno = yylineno;
    yylval.treeptr->leaf->filename = yyfile;

    switch (code)
    {
        case LITERALINT:
            yylval.treeptr->leaf->ival = strtol(yytext, NULL, 10);
            break;
        case LITERALHEX:
            yylval.treeptr->leaf->ival = strtol(yytext+2, NULL, 16);
            break;
        case LITERALFLOAT:
            yylval.treeptr->leaf->dval = strtod(yytext, NULL);
            break;
        case LITERALSTRING:
            yylval.treeptr->leaf->sval = calloc(1, strlen(yytext)); // may be too long, used yyleng
            int walk = 1;
            while (*(yytext+walk) != '\0') {
                if (*(yytext+walk) == '\\') {
                    // handle escape characters
                    walk++;
                    switch (*(yylval.treeptr->leaf->text+walk))
                    {
                    case 'n':
                        *(yylval.treeptr->leaf->sval+walk) = 0x0a;
                        break;
                    case 't':
                        *(yylval.treeptr->leaf->sval+walk) = 0x09;
                        break;
                    case '\\':
                        *(yylval.treeptr->leaf->sval+walk) = 0x5c;
                        break;
                    case '\'':
                        *(yylval.treeptr->leaf->sval+walk) = 0x27;
                        break;
                    case '\"':
                        *(yylval.treeptr->leaf->sval+walk) = 0x22;
                        break;
                    default:
                        printf("Error: Unsupported escape character %s at line %d in file %s\n\n", yytext, yylineno, yyfile);
                        fclose(yyin);
                        exit(1);
                        break;
                    }
                } else {
                    *(yylval.treeptr->leaf->sval+walk) = *(yytext+walk);
                }
                walk++;
            }
            *(yylval.treeptr->leaf->sval+walk) = '\0';
            break;
        case LITERALCHAR:
            if (*(yytext+1) == '\\') {
                // handle escape characters
                switch (*(yylval.treeptr->leaf->text+2))
                {
                case 'n':
                    yylval.treeptr->leaf->ival = 0x0a;
                    break;
                case 't':
                    yylval.treeptr->leaf->ival = 0x09;
                    break;
                case '\\':
                    yylval.treeptr->leaf->ival = 0x5c;
                    break;
                case '\'':
                    yylval.treeptr->leaf->ival = 0x27;
                    break;
                case '\"':
                    yylval.treeptr->leaf->ival = 0x22;
                    break;
                default:
                    printf("Error: unsupported escape character %s at line %d in file %s\n\n", yytext, yylineno, yyfile);
                    break;
                }
            } else {
                yylval.treeptr->leaf->ival = *(yytext+1);
            }
            break;
        default:
            break;
    }

    return 0;
}


/*
 * Initialize and allocate a new tree structure node. returns the new
 * tree structure.
 */
struct tree* allocTree(int code, char* symb, int numkids, ...)
{
    struct tree* tr = (struct tree*)calloc(1, sizeof(struct tree));
    if (!tr) {
        printf("Error: memory allocation for tree node failed.\n\n");
        exit(1);
    }
    // initialize the tree structure node
    tr->prodrule = code;
    tr->symbolname = symb;
    tr->nkids = numkids;

    va_list ap;
    va_start(ap, numkids);

    if (numkids > 0) {
        for (int i=0; i < numkids; i++) {
            // max of 9 kids
            tr->kids[i] = va_arg(ap, struct tree*);
        }
    }

    return tr;
}

/*
 * Takes a tree pointer and prints all values stored within. Nested values are 
 * printed with indentation to signify depth for easier viewing.
 * Returns nothing.  -- WHY WAS IT FREEING VALUES??
 */
void printTree(struct tree *t, int depth)
{
    // if there is no tree structure, return
    if (t == NULL) {
        return;
    }

    // check if t is a leaf node
    if (t->nkids == 0) {
        // print leaf information
        printf("LEAF %*s\t%d: %s  ", depth * 2, " ", t->leaf->category, t->leaf->text);
        // If the ival/sval field is not NULL, print the value to standard output
        if (t->leaf->sval != NULL) {
            printf("%s\n", t->leaf->sval);
        } else if (t->leaf->category == LITERALINT || t->leaf->category == LITERALFLOAT || t->leaf->category == LITERALCHAR) {
            fprintf(stdout, "%d\n", t->leaf->ival);
        }
    } else {
        // print the tree structure information
        printf("TREE %*s %s: %d\n", depth * 2, " ", t->symbolname, t->prodrule);
        // recursive call for each kid
        for (int i = 0; i < t->nkids; i++) {
            printTree(t->kids[i], depth+1);
        }
    }
}

/*
 * Takes a tree pointer and frees the memory allocation for all data values within
 * the given tree structure t.
 * Returns nothing.
 */
void freeTree(struct tree* t)
{
    // if there is no tree structure, return
    if (t == NULL) {
        return;
    }

    //check if t is a leaf node
    if (t->nkids == 0) {
        // free the leaf and tree structures
        if (t->leaf->sval != NULL) {
            free(t->leaf->sval);
        }
        free(t->leaf);
        free(t);
    } else {
        // recursive call for each kid
        for (int i = 0; i < t->nkids; i++) {
            freeTree(t->kids[i]);
        }
        // free the local tree struct
        free(t);
    }
}

/*
 * Takes a defined integer code, a status as a string, and an error type
 * as a string. Logs the thrown error to the console before applying a safe exit
 * from compiler current state.
 * Returns nothing.
 */
void returnOnError(int code, char* status, char* errType) {
    printf("Error::%d:: %s: %s", code, status, errType);
    freeTree(root);
    exit(1);
}

/**
 * The general implementation of yyerror as defined through bison. The function
 * takes a string and prints an error to the standard error before exiting the program.
 * TODO: Reformat so that any memory is safely freed before the exit.
 */
int yyerror(char *s) {
    fprintf(stderr, "%s: %d: Error:: %s\n", yyfile, yylineno, s);
    // Return int value 2 for Syntax Error
	exit(2);
}