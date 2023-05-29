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
%token <treeptr> LBRACKET RBRACKET COMA COLON SEMICOLON PACK MAINPACK PACKNAME
%token <treeptr> MAINFUNC IDENTIFIER USE DROPVAL UNSUPPORTEDOP UNSUPPORTEDKEY
%token <treeptr> STRINGERR CHARERR COMENTERR ESCAPE

%type <treeptr> SourcePack
%type <treeptr> SourceFile
%type <treeptr> PackDecl
%type <treeptr> UseDecls
%type <treeptr> UseDecl
%type <treeptr> FunctionDecls
%type <treeptr> FunctionDecl
%type <treeptr> FunctionHeader
%type <treeptr> FunctionBody
%type <treeptr> FunctionBodyDecls
%type <treeptr> FunctionBodyDecl
%type <treeptr> FunctionReturnVal
%type <treeptr> FieldDecl
%type <treeptr> Type
%type <treeptr> Name
%type <treeptr> VarDecls
%type <treeptr> VarDecl
%type <treeptr> FormalParamListOpt
%type <treeptr> FormalParamList
%type <treeptr> FormalParam
%type <treeptr> ArgListOpt
%type <treeptr> Block
%type <treeptr> BlockStmtsOpt
%type <treeptr> BlockStmts
%type <treeptr> BlockStmt
%type <treeptr> LocalVarDeclsStmt
%type <treeptr> LocalVarDecl
%type <treeptr> Stmt_Wt
%type <treeptr> Stmt
%type <treeptr> ExprStmt
%type <treeptr> StmtExpr
%type <treeptr> IfThenStmt
%type <treeptr> IfThenElseStmt
%type <treeptr> ElseIfSeq
%type <treeptr> ElseIfStmt
%type <treeptr> BreakStmt
%type <treeptr> ReturnStmt
%type <treeptr> Primary
%type <treeptr> Literal
%type <treeptr> ArgList
%type <treeptr> FieldAccess
%type <treeptr> UnaryExpr
%type <treeptr> MultExpr
%type <treeptr> AddExpr
%type <treeptr> RelOp
%type <treeptr> RelExpr
%type <treeptr> EqExpr
%type <treeptr> CondAndExpr
%type <treeptr> CondOrExpr
%type <treeptr> Expr
%type <treeptr> Assign
%type <treeptr> LeftHandSide
%type <treeptr> AssignOp

%%

/* Rules */

SourcePack: SourceFile { root = $1; };
SourceFile: PackDecl UseDecls FunctionDecls
    {}
;

%%

/* Auxiliary Routines  */
