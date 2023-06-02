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
%token <treeptr> MAINFUNC IDENTIFIER RETURNTYPE USE DROPVAL UNSUPPORTEDOP
%token <treeptr> UNSUPPORTEDKEY STRINGERR CHARERR COMENTERR ESCAPE

%type <treeptr> SourcePack
%type <treeptr> SourceFile
%type <treeptr> PackDecl
%type <treeptr> UseDecls
%type <treeptr> UseDecl
%type <treeptr> DataDefDecls
%type <treeptr> StructDecls
%type <treeptr> SymDefDecls
%type <treeptr> SymLitList
%type <treeptr> StructDecl
%type <treeptr> StructBody
%type <treeptr> StructParams
%type <treeptr> StructParam
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

/* -- Source File/Package -- */
SourcePack: SourceFile { root = $1; };
SourceFile: PackDecl UseDecls DataDefDecls FunctionDecls
    {$$ = allocTree(SOURCE_FILE, "source_file", 4, $1, $2, $3, $4);}
;
PackDecl: PACK MAINPACK {$$ = allocTree(PACK_DECL, "pack_decl", 2, $1, $2);}
    | PACK PACKNAME     {$$ = allocTree(PACK_DECL, "pack_decl", 2, $1, $2);}
;
UseDecls: UseDecls UseDecl  {$$ = allocTree(USE_DECLS, "use_decls", 2, $1, $2);}
    | UseDecl               {$$ = allocTree(USE_DECLS, "use_decls", 1, $1);}
;
UseDecl: USE PACKNAME {$$ = allocTree(USE_DECL, "use_decl", 2, $1, $2);}
;
DataDefDecls: SymDefDecls StructDecls {$$ = allocTree(DATA_DEF_DECLS, "data_def_decl", 2, $1, $2);}
    | StructDecls SymDefDecls         {$$ = allocTree(DATA_DEF_DECLS, "data_def_decls", 2, $1, $2);}
;


/* -- Symbol Definitions -- */
SymDefDecls: SYMBOL EQUALTO LBRACKET SymLitList RBRACKET
    {$$ = allocTree(SYM_DEF_DECLS, "sym_def_decls", 1, $4);}
    | SYMBOL IDENTIFIER EQUALTO LBRACKET SymLitList RBRACKET
    {$$ = allocTree(SYM_DEF_DECLS, "sym_def_decls", 2, $2, $5);}
;
SymLitList: SymLitList COMA LITERALSYMBOL {$$ = allocTree(SYM_LIT_LIST, "sym_lit_list", 2, $1, $3);}
    | LITERALSYMBOL {$$ = allocTree(SYM_LIT_LIST, "sym_lit_list", 1, $1);}
;


/* -- Structure Definitions -- */
StructDecls: StructDecls StructDecl      {$$ = allocTree(STRUCT_DECLS, "struct_decls", 2, $1, $2);}
    | StructDecls SymDefDecls StructDecl {$$ = allocTree(STRUCT_DECLS, "struct_decls", 3, $1, $2, $3);}
    | StructDecl {$$ = allocTree(STRUCT_DECLS, "struct_decls", 1, $1);}
;
StructDecl: STRCUCT IDENTIFIER LBRACE StructBody RBRACE SEMICOLON
    {$$ = allocTree(STRUCT_DECL, "struct_decl", 3, $1, $2, $4);}
;
StructBody: StructParams StructParam {$$ = allocTree(STRUCT_PARAMS, "struct_params", 2, $1, $2);}
    | StructParam {$$ = allocTree(STRUCT_PARAMS, "struct_params", 1, $1);}
;
StructParam: IDENTIFIER Type SEMICOLON {$$ = allocTree(STRUCT_PARAM, "struct_param", 2, $1, $2);}


/* -- Function Definitions -- */
FunctionDecls: FunctionDecls FunctionDecl
    {$$ = allocTree(FUNCTION_DECLS, "function_decls", 2, $1, $2);}
    | FunctionDecl {$$ = allocTree(FUNCTION_DECLS, "function_decls", 1, $1);}
;
FunctionDecl: FunctionHeader LBRACKET FunctionBody RBRACKET
    {$$ = allocTree(FUNCTION_DECL, "function_decl", 2, $1, $3);}
;
FunctionHeader: FUNCTION IDENTIFIER RETURNTYPE LPAREN FormalParamListOpt RPAREN
    {$$ = allocTree(FUNCTION_HEADER, "function_header", 4, $1, $2, $3, $5);}
;
FormalParamListOpt: FormalParamList
    {$$ = allocTree(FORMAL_PARAM_LIST_OPT, "formal_param_list_opt", 1, $1);}
;
FormalParamList: FormalParam
    {$$ = allocTree(FORMAL_PARAM_LIST, "formal_param_list", 1, $1);}
    | FormalParamList COMA FormalParam  {$$ = allocTree(FORMAL_PARAM_LIST, "formal_param_list", 2, $1, $3);}
;
FormalParam: IDENTIFIER Type {$$ = allocTree(FORMAL_PARAM, "formal_param", 2, $1, $2);}


/* -- Function Body Definitions -- */
FunctionBody: FunctionReturnVal {}
;

%%

/* Auxiliary Routines  */
