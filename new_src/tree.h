/*
 * File: tree.h
 * Author: Noe Garcia
 * Date: February, 2023
 * Description: Header file contains external variables used for the syntax
 *   tree of the Solace compiler. The following enumeration contains all
 *   integer values corresponding to terminal values. All token and
 *   tree structures are defined bellow. Finally, all tree and token funciton
 *   prototypes are defined at the end of the file.
 */

#ifndef TREE_H
#define TREE_H

#include <stdio.h>

extern FILE* yyin; // !- contains file to compile -!
extern char* yytext;
extern int yylineno;
extern char* yyfile;
extern struct tree* root; // contains the root of the tree generated from parsing


enum arc_terms {
    FILE_ROOT=1001,
    MOD_SPACE,
    MOD_SPACE_DEFINITIONS,
    USE_DEFINITIONS,
    IMPORT_PATH,        // 1005
    IMPORT_LIST,
    STRUCT_DEFINITION,
    STRUCT_PARAMS,
    STRUCT_PARAM,
    FUNC_DEFINITION,    // 1010
    FUNC_HEADER,
    PARAM_LIST_OPT,
    PARAM_LIST,
    PARAM,
    PARAM_MATCH,        // 1015
    FUNC_BODYS,
    FUNC_BODY,
    FUNC_BODY_PATTERN,
    FUNC_BODY_DECLS,
    FUNC_BODY_DECL,
    FUNC_RET_DECL,
    PATTERN_BLOCK,      // 1020
    PATTERN_STMTS,
    PATTERN_STMT,
    COND_STMTS,
    TRAILING_CONDS,
    EXPR,               // 1025
    COND_OR_EXPR,
    COND_AND_EXPR,
    EQ_EXPR,
    RELATION_EXPR,
    ADD_EXPR,           // 1030
    MULT_EXPR,
    UNARY_EXPR,
    CONCAT_EXPR,
    POST_FIX_EXPR,
    VAR_ASSIGNMENT,     // 1035
    LEFT_HAND_SIDE,
    NAME,
    PRIMARY,
    FIELD_ACCESS,
    FUNC_CALL,          // 1040
    LAMBDA_EXPR,
    ARG_LIST_OPT,
    ARG_LIST,
    RELATION_OP,
    TYPE,               // 1045
    TUP_TYPE_DECL,
    LITERAL,
    LIST_LITERALS_OPT,
    LIST_LITERALS,
    SYNTAX_ERROR,       // 1050
};

/* TOKEN AND TREE STRUCTURES */

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

/* FUNCTION PROTOTYPES */

int allocToken(int code);
struct tree* allocTree(int code, char* symb, int numkids, ...);
void printTree(struct tree* t, int depth);
void freeTree(struct tree* t);
void returnOnError(int code, char* status, char* errType);

// Dependant functions
int yylex_destroy();
extern int yyparse();

#endif