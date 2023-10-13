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
    SOURCE_FILE=1001,       // 1001
    MOD_DECL,
    USE_DECL,
    IMPORT_LIST,
    STRUCT_DECL,            // 1005
    STRUCT_BODY,
    STRUCT_PARAMS,
    STRUCT_PARAM,
    FUNCTION_DECL,
    FUNCTION_HEADER,        // 1010
    FUNCTION_BODY,
    FUNCTION_BODY_DECLS,
    FUNCTION_BODY_DECL,
    LOCAL_NAME_CALL,
    LOCAL_NAME_DECL,        // 1015
    SPACE_FUNC_CALL,
    PATTERN_BLOCKS,
    PATTERN_BLOCK,
    MOD_NAME,
    TYPE,                   // 1020
    NAME,
    VAR_ASSIGNMENT,
    STRUCT_VAR_DECL,
    STRUCT_VAR_PARAMS,
    FORMAL_PARAM_LIST_OPT,  // 1025
    FORMAL_PARAM_LIST,
    FORMAL_PARAM,
    ARG_LIST_OPT,
    ARG_LIST,
    ARG_VAL,                // 1030
    PRIMARY,
    TUPPLE_TYPE,
    TUPPLE_DECL,
    TUPPLE_CONST,
    TUPPLE_CONST_VALS,      // 1035
    LITERAL,
    FIELD_ACCESS,
    FIELD,
    POST_FIX_EXPR,
    UNARY_EXPR,             // 1040
    MULT_EXPR,
    ADD_EXPR,
    REL_OP,
    REL_EXPR,
    EQ_EXPR,                // 1045
    COND_AND_EXPR,
    COND_OR_EXPR,
    CONCAT_EXPRS,
    CONCAT_EXPR,
    EXPR,                   // 1050
    LEFT_HAND_SIDE,
    ASSIGN_OP,
    SYNTAX_ERROR,           // 1053
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