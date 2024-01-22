/*
 * File: tree.h
 * Author: Noe Garcia
 * Date: February, 2023
 * Description: Header file contains external variables used for the syntax
 *  tree of the Solace compiler. The following enumeration contains all
 *  integer values corresponding to terminal values. Finally, all token and
 *  tree structures are defined.
 */
#include <stdio.h>

extern FILE* yyin; // !- contains file to compile -!
extern char* yytext;
extern int yylineno;
extern char* yyfile;
// contains the root of the tree generated from parsing
extern struct tree* root;

enum sol_terms {
    FILE_ROOT=1001,
    SOURCE_SPACE,
    FILE_DEFINITIONS,
    USE_DEFINITIONS,
    IMPORT_LIST,        // 1005
    STRUCT_DEFINITION,
    STRUCT_PARAMS,
    STRUCT_PARAM,
    FUNC_DEFINITION,
    FUNC_HEADER,        // 1010
    PARAM_LIST_OPT,
    PARAM_LIST,
    PARAM,
    FUNC_BODY,
    FUNC_BODY_DECLS,    // 1015
    FUNC_BODY_DECL,
    PATTERN_BLOCK,
    PATTERN_STMT,
    EXPR,
    COND_OR_EXPR,       // 1020
    COND_AND_EXPR,
    EQ_EXPR,
    RELATION_EXPR,
    ADD_EXPR,
    MULT_EXPR,          // 1025
    UNARY_EXPR,
    CONCAT_EXPR,
    POST_FIX_EXPR,
    VAR_ASSIGNMENT,
    LEFT_HAND_SIDE,     // 1030
    NAME,
    PRIMARY,
    FIELD_ACCESS,
    FUNC_CALL,
    LAMBDA_EXPR,        // 1035
    ARG_LIST_OPT,
    ARG_LIST,
    RELATION_OP,
    TYPE,
    TUP_TYPE_DECL,      // 1040
    LITERAL,
    LIST_LITERALS_OPT,
    LIST_LITERALS,
    SYNTAX_ERROR,
};

// Code Structures

struct token {
    int category;   // integer return code from yylex
    char* text;     // raw string pattern
    int lineno;     // token line number location
    char* filename; // source file name

    int ival;       // used for integer values
    double dval;    // used for floating point values
    char* sval;     // used for character and string values
};

struct tree {
    int prodrule;
    char* symbolname;
    int nkids;
    
    struct tree* kids[9]; // if nkids > 0
    struct token* leaf;   // if nkids == 0; Null for ε productions
};

// Function Prototypes

int allocToken(int code);
struct tree* allocTree(int code, char* symb, int numkids, ...);
void printTree(struct tree* t, int depth);
void freeTree(struct tree* t);
void returnOnError(int code, char* status, char* errType);

// Dependant functions
int yylex_destroy();
extern int yyparse();