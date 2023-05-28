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

%token <treeptr> BOOLEAN INT FLOAT CHAR STRING SYMBOL
%token <treeptr> LITERALBOOL LITERALINT LITERALHEX LITERALFLOAT
%token <treeptr> LITERALCHAR LITERALSTRING LITERALSYMBOL FUNCTION TUPPLE
%token <treeptr> ARRAY STRCUCT ADD SUBTRACT MULTIPLY DIVIDE MODULO
%token <treeptr> ASSIGNMENT BAR PATTERNMATCH CONDSTATEMENT ARROWOP RETURN
%token <treeptr> EQUALTO NOTEQUAL COMPARISON LBRACE RBRACE LPAREN RPAREN
%token <treeptr> LBRACKET RBRACKET COMA COLON SEMICOLON PACK MAINMOD
%token <treeptr> MAINFUNC IDENTIFIER USE DROPVAL UNSUPPORTEDOP UNSUPPORTEDKEY
%token <treeptr> STRINGERR CHARERR COMENTERR ESCAPE

%type <treeptr> FUNCTION_DECL
%type <treeptr> FUNCTION_HEADER
%type <treeptr> FUNCTION_BODY
%type <treeptr> FUNCTION_BODY_DECLS
%type <treeptr> FUNCTION_BODY_DECL
%type <treeptr> FUNCTION_RETURN_VAL
%type <treeptr> FIELD_DECL
%type <treeptr> TYPE
%type <treeptr> NAME
%type <treeptr> VAR_DECLS
%type <treeptr> VAR_DECL
%type <treeptr> FORMAL_PARAM_LIST_OPT
%type <treeptr> FORMAL_PARAM_LIST
%type <treeptr> FORMAL_PARAM
%type <treeptr> ARG_LIST_OPT
%type <treeptr> BLOCK
%type <treeptr> BLOCK_STMTS_OPT
%type <treeptr> BLOCK_STMTS
%type <treeptr> BLOCK_STMT
%type <treeptr> LOCAL_VAR_DECL_STMT
%type <treeptr> LOCAL_VAR_DECL
%type <treeptr> STMT_WT
%type <treeptr> STMT
%type <treeptr> EXPR_STMT
%type <treeptr> STMT_EXPR
%type <treeptr> IF_THEN_STMT
%type <treeptr> IF_THEN_ELSE_STMT
%type <treeptr> ELSE_IF_SEQ
%type <treeptr> ELSE_IF_STMT
%type <treeptr> BREAK_STMT
%type <treeptr> RETURN_STMT
%type <treeptr> PRIMARY
%type <treeptr> LITERAL
%type <treeptr> ARG_LIST
%type <treeptr> FIELD_ACCESS
%type <treeptr> UNARY_EXPR
%type <treeptr> MULT_EXPR
%type <treeptr> ADD_EXPR
%type <treeptr> REL_OP
%type <treeptr> REL_EXPR
%type <treeptr> EQ_EXPR
%type <treeptr> COND_AND_EXPR
%type <treeptr> COND_OR_EXPR
%type <treeptr> EXPR
%type <treeptr> ASSIGN
%type <treeptr> LEFT_HAND_SIDE
%type <treeptr> ASSIGN_OP

%%

/* Rules */

%%

/* Auxiliary Routines  */
