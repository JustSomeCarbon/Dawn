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
    USE_DECLS,
    USE_DECL,
    DATA_DEF_DECLS,
    SYM_DEF_DECLS,
    SYM_LIT_LIST,
    STRUCT_DECLS,
    STRUCT_DECL,
    STRUCT_BODY,
    STRUCT_PARAMS,
    STRUCT_PARAM,
    FUNCTION_DECLS,
    FUNCTION_DECL,
    FUNCTION_HEADER,
    FUNCTION_BODY,
    FUNCTION_BODY_DECLS,
    FUNCTION_BODY_DECL,
    FUNCTION_RETURN_VAL,
    FIELD_DECL,
    TYPE,
    NAME,
    VAR_DECLS,
    VAR_DECL,
    FORMAL_PARAM_LIST_OPT,
    FORMAL_PARAM_LIST,
    FORMAL_PARAM,
    ARG_LIST_OPT,
    BLOCK,
    BLOCK_STMTS_OPT,
    BLOCK_STMTS,
    BLOCK_STMT,
    LOCAL_VAR_DECL_STMT,
    LOCAL_VAR_DECL,
    STMT_WT,
    STMT,
    EXPR_STMT,
    STMT_EXPR,
    PATTERN_BLOCK,
    IF_THEN_STMT,
    IF_THEN_ELSE_STMT,
    ELSE_IF_SEQ,
    ELSE_IF_STMT,
    BREAK_STMT,
    RETURN_STMT,
    PRIMARY,
    LITERAL,
    ARG_LIST,
    FIELD_ACCESS,
    POST_FIX_EXPR,
    UNARY_EXPR,
    MULT_EXPR,
    ADD_EXPR,
    REL_OP,
    REL_EXPR,
    EQ_EXPR,
    COND_AND_EXPR,
    COND_OR_EXPR,
    EXPR,
    ASSIGN,
    LEFT_HAND_SIDE,
    ASSIGN_OP,
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
