/*
 * file: tree.c
 * author: Noe Garcia
 * description: Contains all functions regarding the syntax tree construction
 *  and traversal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "sgram.tab.h"

/*
 * Initialize and allocate a new token structure based on the given code value.
 * When complete, returns the code as an integer.
 */
int allocToken(int code)
{
    // initialize the memory for a new tree-leaf node
    yylval.treeptr = calloc(1, sizeof(struct tree));
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
            yylval->treeptr->leaf->ival = strtol(yytext, NULL, 10);
            break;
        case LITERALHEX:
            yylval->treeptr->leaf->ival = strtol(yytext+2, NULL, 16);
            break;
        case LITERALFLOAT:
            yylval->treeptr->leaf->dval = strtod(yytext, NULL);
            break;
        case LITERALSTRING:
            yylval->treeptr->leaf->sval = calloc(1, strlen(yytext)); // may be too long, used yyleng
            int walk = 1;
            while (*(yytext+walk) != '\0') {
                if (*(yytext+walk) == '\\') {
                    // handle escape characters
                    walk++;
                    switch (*(yylval->treeptr->leaf->text+walk))
                    {
                    case 'n':
                        *(yylval->treeptr->leaf->sval+walk) = 0x0a;
                        break;
                    case 't':
                        *(yylval->treeptr->leaf->sval+walk) = 0x09;
                        break;
                    case '\\':
                        *(yylval->treeptr->leaf->sval+walk) = 0x5c;
                        break;
                    case '\'':
                        *(yylval->treeptr->leaf->sval+walk) = 0x27;
                        break;
                    case '\"':
                        *(yylval->treeptr->leaf->sval+walk) = 0x22;
                        break;
                    default:
                        printf("Error: Unsupported escape character %s at line %d in file %s\n\n", yytext, yylineno, argv[filearg]);
                        fclose(yyin);
                        exit(1);
                        break;
                    }
                } else {
                    *(yylval->treeptr->leaf->sval+walk) = *(yytext+walk);
                }
                walk++;
            }
            *(yylval->treeptr->leaf->sval+walk) = '\0';
            break;
        case LITERALCHAR:
            if (*(yytext+1) == '\\') {
                // handle escape characters
                switch (*(yylval->treeptr->leaf->text+2))
                {
                case 'n':
                    yylval->treeptr->leaf->ival = 0x0a;
                    break;
                case 't':
                    yylval->treeptr->leaf->ival = 0x09;
                    break;
                case '\\':
                    yylval->treeptr->leaf->ival = 0x5c;
                    break;
                case '\'':
                    yylval->treeptr->leaf->ival = 0x27;
                    break;
                case '\"':
                    yylval->treeptr->leaf->ival = 0x22;
                    break;
                default:
                    printf("Error: unsupported escape character %s at line %d in file %s\n\n", yytext, yylineno, argv[filearg]);
                    break;
                }
            } else {
                yylval->treeptr->leaf->ival = *(yytext+1);
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
    for (int i=0; i < numkids; i++) {
        // max of 9 kids
        tr->kids[i] = va_arg(ap, struct tree*);
    }

    return tr;
}