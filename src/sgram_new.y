// Solace Grammar yacc definition file

%{
    #include <stdio.h>
    #include "tree.h"
    extern int yylex(void);
    extern void yyerror(char const *);

    struct tree* root;
%}

%error-verbose

%union {
    struct tree* treeptr;
}

%token <treeptr> LBRACKET RBRACKET COMA COLON SEMICOLON MODSPACE
%token <treeptr> LBRACE RBRACE LPAREN RPAREN
%token <treeptr> MAINFUNC IDENTIFIER USE DROPVAL
%token <treeptr> BOOLEAN INT FLOAT CHAR STRING SYMBOL HEADVAR TAILVAR
%token <treeptr> LITERALBOOL LITERALINT LITERALHEX LITERALFLOAT
%token <treeptr> LITERALCHAR LITERALSTRING LITERALSYMBOL FUNCTION
%token <treeptr> STRUCT ADD SUBTRACT MULTIPLY DIVIDE MODULO
%token <treeptr> ASSIGNMENT BAR ARROWOP RETURN DOT
%token <treeptr> ISEQUALTO NOTEQUALTO LOGICALAND LOGICALOR NOT
%token <treeptr> GREATERTHANOREQUAL LESSTHANOREQUAL GREATERTHAN LESSTHAN

%type <treeptr> FileRoot
%type <treeptr> SourceSpace
%type <treeptr> FileDefinitions

%type <treeptr> UseDefinition
%type <treeptr> ImportList

%type <treeptr> StructDefinition

%type <treeptr> FunctionDefinition
%type <treeptr> FunctionHeader
%type <treeptr> ParameterListOpt
%type <treeptr> ParameterList
%type <treeptr> FunctionBody
%type <treeptr> FunctionBodyDecls
%type <treeptr> FunctionBodyDecl

%type <treeptr> Expr
%type <treeptr> CondOrExpr
%type <treeptr> CondAndExpr
%type <treeptr> EqExpr
%type <treeptr> RelationExpr
%type <treeptr> AddExpr
%type <treeptr> MultExpr
%type <treeptr> UnaryExpr
%type <treeptr> PostFixExpr

%type <treeptr> VarAssignment
%type <treeptr> LeftHandSide

%type <treeptr> Name
%type <treeptr> Primary
%type <treeptr> FunctionCall
%type <treeptr> ArgumentListOpt
%type <treeptr> ArgumentList

%type <treeptr> RelationOp
%type <treeptr> Type
%type <treeptr> Literal

%%

/* Gramatical rules */

/* -- SOURCE FILE DEFINITION -- */

FileRoot: SourceSpace FileDefinitions {$$ = allocTree(FILE_ROOT, "file_root", 2, $1, $2);}
;
SourceSpace: MODSPACE COLON Name {$$ = allocTree(SOURCE_SPACE, "source_space", 2, $1, $3);}
;
FileDefinitions: FileDefinitions StructDefinition {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 2, $1, $2);}
    | FileDefinitions UseDefinition      {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 2, $1, $2);}
    | FileDefinitions FunctionDefinition {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 2, $1, $2);}
    | UseDefinition      {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 1, $1);}
    | StructDefinition   {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 1, $1);}
    | FunctionDefinition {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 1, $1);}
    | {/* - NO VALUE - */}
;

/*  -- USE GRAMAR DEFINITIONS -- */

UseDefinition: USE Name SEMICOLON {$$ = allocTree();}
    | USE Name COLON LBRACE RBRACE {$$ = allocTree();}
;
ImportList: ImportList COMA Name {$$ = allocTree();}
    | Name {$$ = allocTree();}
;


/* -- STRUCT GRAMAR DEFINITIONS -- */

StructDefinition: STRUCT Name LBRACE ParameterList RBRACE {$$ = allocTree();}
;


/*  -- FUNCTION DECLARATION GRAMAR DEFINITIONS -- */

FunctionDefinition: FunctionHeader FunctionBody {$$ = allocTree();}
;
FunctionHeader: FUNCTION Name Type ParameterListOpt {$$ = allocTree();}
;
ParameterListOpt: LPAREN ParameterList RPAREN {$$ = allocTree();}
    | LPAREN RPAREN {/* - NO VALUE - */}
;
ParameterList: ParameterList COMA Name Type {$$ = allocTree();}
    | Name Type {$$ = allocTree();}
;
FunctionBody: LBRACE FunctionBodyDecls RBRACE {$$ = allocTree();}
    | LBRACE RBRACE {/* - NO VALUE - */}
;
FunctionBodyDecls: FunctionBodyDecls FunctionBodyDecl {$$ = allocTree();}
    | FunctionBodyDecl {$$ = allocTree();}
;
FunctionBodyDecl: Expr {$$ = allocTree();}
;


/* -- EXPRESSION GRAMAR DEFINITIONS -- */

Expr: CondOrExpr    {$$ = allocTree();}
    | VarAssignment {$$ = allocTree();}
;
CondOrExpr: CondOrExpr LOGICALOR CondAndExpr {$$ = allocTree();}
    | CondAndExpr {$$ = allocTree();}
;
CondAndExpr: CondAndExpr LOGICALAND EqExpr {$$ = allocTree();}
    | EqExpr {$$ = allocTree();}
;
EqExpr: EqExpr ISEQUALTO RelationExpr  {$$ = allocTree();}
    | EqExpr NOTEQUALTO RelationExpr {$$ = allocTree();}
    | RelationExpr                   {$$ = allocTree();}
;
RelationExpr: RelationExpr RelationOp AddExpr {$$ = allocTree();} 
    | AddExpr {$$ = allocTree();}
;
AddExpr: AddExpr ADD MultExpr   {$$ = allocTree();}
    | AddExpr SUBTRACT MultExpr {$$ = allocTree();}
    | MultExpr                  {$$ = allocTree();}
;
MultExpr: MultExpr MULTIPLY UnaryExpr {$$ = allocTree();}
    | MultExpr DIVIDE UnaryExpr       {$$ = allocTree();}
    | MultExpr MODULO UnaryExpr       {$$ = allocTree();}
    | UnaryExpr                       {$$ = allocTree();}
;
UnaryExpr: NOT UnaryExpr {$$ = allocTree();}
    | SUBTRACT UnaryExpr {$$ = allocTree();}
    | PostFixExpr        {$$ = allocTree();}
;
PostFixExpr: Primary {$$ = allocTree();}
    | Name           {$$ = allocTree();}
;


/* -- ASSIGNMENT AND OPERATION GRAMAR DEFINITIONS --  */
VarAssignment: LeftHandSide ASSIGNMENT Expr {$$ = allocTree();}
;
LeftHandSide: Name Type {$$ = allocTree();}
;


/* -- NAMES AND FUNCTION CALLS GRAMMAR DEFINITIONS -- */

Name: Name COLON IDENTIFIER {$$ = allocTree();}
    | IDENTIFIER            {$$ = allocTree();}
;
Primary: Literal         {$$ = allocTree();}
    | LPAREN Expr RPAREN {$$ = allocTree();}
    | FunctionCall       {$$ = allocTree();}
;
FunctionCall: Name ArgumentListOpt {$$ = allocTree();}
;
ArgumentListOpt: LPAREN ArgumentList RPAREN {$$ = allocTree();}
    | LPAREN RPAREN {/* - NO VALUE - */}
;
ArgumentList: ArgumentList COMA Expr {$$ = allocTree();}
    | Expr {$$ = allocTree();}
;


/*  -- TYPES AND LITERALS GRAMAR DEFINITIONS --  */

RelationOp: GREATERTHAN  {$$ = allocTree();}
    | GREATERTHANOREQUAL {$$ = allocTree();}
    | LESSTHAN           {$$ = allocTree();}
    | LESSTHANOREQUAL    {$$ = allocTree();}
;
Type: BOOLEAN  {$$ = allocTree();}
    | INT      {$$ = allocTree();}
    | FLOAT    {$$ = allocTree();}
    | CHAR     {$$ = allocTree();}
    | STRING   {$$ = allocTree();}
    | SYMBOL   {$$ = allocTree();}
    | FUNCTION {$$ = allocTree();}
;
Literal: LITERALBOOL {$$ = allocTree();}
    | LITERALINT     {$$ = allocTree();}
    | LITERALFLOAT   {$$ = allocTree();}
    | LITERALCHAR    {$$ = allocTree();}
    | LITERALSTRING  {$$ = allocTree();}
    | LITERALSYMBOL  {$$ = allocTree();}
    | LITERALHEX     {$$ = allocTree();}
;

%%