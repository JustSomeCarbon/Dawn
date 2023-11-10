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

%token <treeptr> BOOLEAN INT FLOAT CHAR STRING SYMBOL HEADVAR TAILVAR
%token <treeptr> LITERALBOOL LITERALINT LITERALHEX LITERALFLOAT
%token <treeptr> LITERALCHAR LITERALSTRING LITERALSYMBOL FUNCTION
%token <treeptr> STRUCT ADD SUBTRACT MULTIPLY DIVIDE MODULO
%token <treeptr> ASSIGNMENT BAR ARROWOP RETURN DOT
%token <treeptr> EQUALTO LBRACE RBRACE LPAREN RPAREN
%token <treeptr> LBRACKET RBRACKET COMA COLON SEMICOLON MODSPACE
%token <treeptr> MAINFUNC IDENTIFIER USE DROPVAL
%token <treeptr> ISEQUALTO NOTEQUALTO LOGICALAND LOGICALOR NOT INCREMENT DECREMENT
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

%type <treeptr> Type

%%

/* Gramatical rules */

/* -- SOURCE FILE DEFINITION -- */

FileRoot: SourceSpace FileDefinitions {$$ = allocTree(FILE_ROOT, "file_root", 2, $1, $2);}
;
SourceSpace: MODSPACE COLON IDENTIFIER {$$ = allocTree(SOURCE_SPACE, "source_space", 2, $1, $3);}
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

UseDefinition: USE IDENTIFIER SEMICOLON {$$ = allocTree();}
    | USE IDENTIFIER COLON LBRACE RBRACE {$$ = allocTree();}
;
ImportList: ImportList COMA IDENTIFIER {$$ = allocTree();}
    | IDENTIFIER {$$ = allocTree();}
;


/* -- STRUCT GRAMAR DEFINITIONS -- */

StructDefinition: STRUCT IDENTIFIER LBRACE ParameterList RBRACE {$$ = allocTree();}
;


/*  -- FUNCTION GRAMAR DEFINITIONS -- */

FunctionDefinition: FunctionHeader FunctionBody {$$ = allocTree();}
;
FunctionHeader: FUNCTION IDENTIFIER Type ParameterListOpt {$$ = allocTree();}
;
ParameterListOpt: LPAREN ParameterList RPAREN {$$ = allocTree();}
    | LPAREN RPAREN {/* - NO VALUE - */}
;
ParameterList: ParameterList COMA IDENTIFIER Type {$$ = allocTree();}
    | IDENTIFIER Type {$$ = allocTree();}
;
FunctionBody:
;


/*  -- TYPE GRAMAR DEFINITIONS --  */

Type:
;

%%