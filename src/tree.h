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
    SOURCE_FILE=1001,
    PACK_DECL,
    USE_DECL,
    STRUCT_DECL,
    STRUCT_BODY,
    STRUCT_PARAMS,
    STRUCT_PARAM,
    FUNCTION_DECL,
    FUNCTION_HEADER,
    FUNCTION_BODY,
    FUNCTION_BODY_DECLS,
    FUNCTION_BODY_DECL,  // 1015
    LOCAL_NAME_CALL,
    LOCAL_NAME_DECL,
    PATTERN_BLOCKS,
    PATTERN_BLOCK,
    PACK_NAME,            // 1020
    TYPE,
    NAME,
    VAR_ASSIGNMENT,
    STRUCT_VAR_DECL,
    STRUCT_VAR_PARAMS,    // 1030
    FORMAL_PARAM_LIST_OPT,
    FORMAL_PARAM_LIST,
    FORMAL_PARAM,
    ARG_LIST_OPT,
    ARG_LIST,             // 1035
    ARG_VAL,
    PRIMARY,
    TUPPLE_TYPE,
    TUPPLE_DECL,
    TUPPLE_CONST,         // 1040
    TUPPLE_CONST_VALS,
    LITERAL,
    FIELD_ACCESS,
    FIELD,
    POST_FIX_EXPR,        // 1045
    UNARY_EXPR,
    MULT_EXPR,
    ADD_EXPR,
    REL_OP,
    REL_EXPR,             // 1050
    EQ_EXPR,
    COND_AND_EXPR,
    COND_OR_EXPR,
    CONCAT_EXPRS,
    CONCAT_EXPR,          // 1055
    EXPR,
    LEFT_HAND_SIDE,
    ASSIGN_OP,
    SYNTAX_ERROR,         // 1060
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