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
    return;
}