/*
 * file: token.c
 * author: noe garcia
 * description: Contains function implementations for token structs.
 */

#include <stdio.h>
#include <stdlib.h>
#include "solace.h"

struct tokenlist* allocateToken()
{
    struct tokenlist* tl = calloc(1, sizeof(struct tokenlist));
    if (tl == NULL) {
        printf("Error: token list memory allocation error\n\n");
        exit(1);
    }

    tl->t = calloc(1, sizeof(struct token));
    if (tl->t == NULL) {
        printf("Error: token memory allocation error\n\n");
        exit(1);
    }
    tl->t->sval = NULL;
    tl->next = NULL;

    return tl;
}

void printTokenList(struct tokenlist* tl)
{
    printf("Category\tText\tLineno\tFilename\tIval\tSval\n");
    printf("-----------------------------------------------------------\n");
    struct tokenlist* tmp = tl;
    while (tl->next != NULL) {
        // print the values to the console
        fprintf(stdout, "%d\t\t", tl->t->category);
        fprintf(stdout, "%s\t", tl->t->text);
        fprintf(stdout, "%d\t", tl->t->lineno);
        fprintf(stdout, "%s\t", tl->t->filename);
        if (tl->t->sval != NULL) {
            fprintf(stdout, "\t\t%s\n", tl->t->sval);
            free(tl->t->sval);
        } else if (tl->t->category == LITERALINT || tl->t->category == LITERALFLOAT || tl->t->category == LITERALCHAR) {
            fprintf(stdout, "\t%d\n", tl->t->ival);
        } else {
            fprintf(stdout, "\n");
        }
        // move pointer and free memory
        tl = tl->next;
        free(tmp->t->text);
        //free(tmp->t->filename);
        free(tmp->t);
        free(tmp);
        tmp = tl;
    }
    free(tl->t->text);
    //free(tl->t->filename);
    free(tl->t);
    free(tl);

    return;
}