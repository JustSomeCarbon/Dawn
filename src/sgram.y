/* Definitions */

%{
    #include <stdio.h>
    #include "tree.h"
    extern int yylex(void);
    extern void yyerror(char const *);
%}

%define parse.lac full      // required for verbose error reporting
%define parse.error verbose // required for detailed error messages

%union {
    struct tree* treeptr;
}

%%

/* Rules */

%%

/* Auxiliary Routines  */
