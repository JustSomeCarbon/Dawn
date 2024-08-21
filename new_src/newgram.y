// Artifice Language Grammar File

%{
    #include <stdio.h>
    #include "tree.h"
    extern int yylex(void);
    extern void yyerror(char const *);

    struct tree* root;
%}

//%error-verbose
//%define parse.error verbose

%union {
    struct tree* treeptr;
}

%token <treeptr> MODSPACE USE DEFINE DO END
%token <treeptr> IDENTIFIER STRUCT FUNCTION MAINFUNC
%token <treeptr> COLON COMA LBRACE RBRACE LBRACKET RBRACKET LPAREN RPAREN
%token <treeptr> BOOLEAN INT FLOAT CHAR STRING SYMBOL
%token <treeptr> LOGICALOR LOGICALAND ISEQUALTO NOTEQUALTO NOT
%token <treeptr> GREATERTHAN GREATERTHANOREQUAL LESSTHAN LESSTHANOREQUAL
%token <treeptr> ADD SUBTRACT MULTIPLY DIVIDE MODULO BAR
%token <treeptr> LITERALBOOL LITERALINT LITERALFLOAT LITERALCHAR
%token <treeptr> LITERALSTRING LITERALSYMBOL LITERALHEX
%token <treeptr> MATCHTO /*ORMATCH*/ RETURN

/*
%token <treeptr> LBRACKET RBRACKET COMA COLON SEMICOLON
%token <treeptr> LBRACE RBRACE LPAREN RPAREN
%token <treeptr> MAINFUNC USE DROPVAL DOT SELF ON
%token <treeptr> BOOLEAN INT FLOAT CHAR STRING SYMBOL FUNCTION
%token <treeptr> THEN IF ELSEIF ELSE ORMATCH MATCH RETURN
%token <treeptr> LITERALBOOL LITERALINT LITERALHEX LITERALFLOAT
%token <treeptr> LITERALCHAR LITERALSTRING LITERALSYMBOL DEFINE
%token <treeptr> STRUCT ADD SUBTRACT MULTIPLY DIVIDE MODULO
%token <treeptr> ASSIGNMENT BAR ARROWOP MATCHTO
%token <treeptr> ISEQUALTO NOTEQUALTO LOGICALAND LOGICALOR NOT
%token <treeptr> GREATERTHANOREQUAL LESSTHANOREQUAL GREATERTHAN LESSTHAN
*/

%type <treeptr> FileRoot
%type <treeptr> ModSpace
%type <treeptr> ModSpaceDefinitions

%type <treeptr> UseDefinition
%type <treeptr> ImportPath
%type <treeptr> ImportList


%type <treeptr> StructDefinition
%type <treeptr> StructParams
%type <treeptr> StructParam

%type <treeptr> FunctionDefinition
%type <treeptr> FunctionHeader
%type <treeptr> ParameterListOpt
%type <treeptr> ParameterList
%type <treeptr> Parameter
%type <treeptr> ParameterMatch
//%type <treeptr> FunctionBodys
%type <treeptr> FunctionBody
//%type <treeptr> FunctionBodyPattern
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
%type <treeptr> ConcatExpr
%type <treeptr> PostFixExpr

%type <treeptr> Name
%type <treeptr> Primary

%type <treeptr> RelationOp
%type <treeptr> Type
%type <treeptr> Literal
%type <treeptr> ListLiteralsOpt
%type <treeptr> ListLiterals

/*
%type <treeptr> FunctionDefinition
%type <treeptr> FunctionHeader
%type <treeptr> ParameterListOpt
%type <treeptr> ParameterList
%type <treeptr> Parameter
%type <treeptr> ParameterMatch
%type <treeptr> FunctionBodys
%type <treeptr> FunctionBody
%type <treeptr> FunctionBodyPattern
%type <treeptr> FunctionBodyDecls
%type <treeptr> FunctionBodyDecl
%type <treeptr> FunctionReturnDecl

%type <treeptr> PatternBlock
%type <treeptr> PatternStmts
%type <treeptr> PatternStmt

%type <treeptr> CondStmts
%type <treeptr> TrailingConds

%type <treeptr> Expr
%type <treeptr> CondOrExpr
%type <treeptr> CondAndExpr
%type <treeptr> EqExpr
%type <treeptr> RelationExpr
%type <treeptr> AddExpr
%type <treeptr> MultExpr
%type <treeptr> UnaryExpr
%type <treeptr> ConcatExpr
%type <treeptr> PostFixExpr

%type <treeptr> VarAssignment
%type <treeptr> LeftHandSide

%type <treeptr> Name
%type <treeptr> Primary
%type <treeptr> FieldAccess
%type <treeptr> FunctionCall
%type <treeptr> LambdaExpr
%type <treeptr> ArgumentListOpt
%type <treeptr> ArgumentList

%type <treeptr> RelationOp
%type <treeptr> Type
%type <treeptr> TupTypeDecl
%type <treeptr> Literal
%type <treeptr> ListLiteralsOpt
%type <treeptr> ListLiterals
*/

%%

/* -- File Root -- */

FileRoot: ModSpace ModSpaceDefinitions END {root = allocTree(FILE_ROOT, "file_root", 2, $1, $2);}
    | ModSpace END                    {root = allocTree(FILE_ROOT, "file_root", 1, $1);}
;
ModSpace: MODSPACE IDENTIFIER {$$ = allocTree(MOD_SPACE, "source_space", 1, $2);}
;
ModSpaceDefinitions: ModSpaceDefinitions UseDefinition  {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 2, $1, $2);}
    | ModSpaceDefinitions DEFINE StructDefinition       {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 2, $1, $3);}
    | ModSpaceDefinitions DEFINE FunctionDefinition     {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 2, $1, $3);}
    | UseDefinition             {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 1, $1);}
    | DEFINE StructDefinition   {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 1, $1);}
    | DEFINE FunctionDefinition {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 1, $1);}
;


/*  -- MOD INCLUSION GRAMAR DEFINITION -- */

UseDefinition: USE ImportPath                 {$$ = allocTree(USE_DEFINITIONS, "use_definitions", 1, $2);}
    | USE ImportPath COLON LBRACE ImportList RBRACE {$$ = allocTree(USE_DEFINITIONS, "use_definitions", 2, $2, $5);}
;
ImportPath: IDENTIFIER {$$ = allocTree(IMPORT_PATH, "import_path", 1, $1);}
    | ImportPath COLON IDENTIFIER {$$ = allocTree(IMPORT_PATH, "import_path", 2, $1, $3);}
;
ImportList: ImportList COMA IDENTIFIER {$$ = allocTree(IMPORT_LIST, "import_list", 2, $1, $3);}
    | IDENTIFIER {$$ = allocTree(IMPORT_LIST, "import_list", 1, $1);}
;


/* -- STRUCT GRAMAR DEFINITIONS -- */

StructDefinition: Name STRUCT LBRACE StructParams RBRACE {$$ = allocTree(STRUCT_DEFINITION, "struct_definition", 2, $2, $5);}
;
StructParams: StructParams COMA StructParam {$$ = allocTree(STRUCT_PARAMS, "struct_params", 2, $1, $3);}
    | StructParam {$$ = allocTree(STRUCT_PARAMS, "struct_params", 1, $1);}
;
StructParam: Name Type {$$ = allocTree(STRUCT_PARAM, "struct_param", 2, $1, $2);}
;


/* -- FUNCTION GRAMMAR DEFINITIONS -- */

FunctionDefinition: FunctionHeader FunctionBody {$$ = allocTree(FUNC_DEFINITION, "func_definition", 2, $1, $2);}
;
FunctionHeader: Name ParameterListOpt Type {$$ = allocTree(FUNC_HEADER, "func_header", 3, $1, $2, $3);}
    | MAINFUNC ParameterListOpt Type       {$$ = allocTree(FUNC_HEADER, "func_header", 3, $1, $2, $3);}
;
ParameterListOpt: LPAREN ParameterList RPAREN {$$ = allocTree(PARAM_LIST_OPT, "param_list_opt", 1, $2);}
    | LPAREN RPAREN {$$ = NULL;}
;
ParameterList: ParameterList COMA Parameter {$$ = allocTree(PARAM_LIST, "param_list", 2, $1, $3);}
    | Parameter {$$ = allocTree(PARAM_LIST, "param_list", 1, $1);}
;
Parameter: Name Type {$$ = allocTree(PARAM, "parameter", 2, $1, $2);}
    | ParameterMatch {$$ = allocTree(PARAM, "parameter", 1, $1);}
;
ParameterMatch: Name Type MATCHTO Expr {$$ = allocTree(PARAM_MATCH, "param_match", 3, $1, $2, $4);}
;
//FunctionBodys: FunctionBody              {$$ = allocTree(FUNC_BODYS, "func_bodys", 1, $1);}
//    | FunctionBodys ORMATCH FunctionBody {$$ = allocTree(FUNC_BODYS, "func_bodys", 2, $1, $3);}
//;
FunctionBody: DO FunctionBodyDecls END {$$ = allocTree(FUNC_BODY, "func_body", 1, $2);}
    | DO END {$$ = NULL;}
;
/*
FunctionBodyPattern: ParameterListOpt Type FunctionBody {$$ = allocTree(FUNC_BODY_PATTERN, "func_body_pattern", 3, $1, $2, $3);}
;
*/
FunctionBodyDecls: FunctionBodyDecls FunctionBodyDecl {$$ = allocTree(FUNC_BODY_DECLS, "func_body_decls", 2, $1, $2);}
    | FunctionBodyDecl    {$$ = allocTree(FUNC_BODY_DECLS, "func_body_decls", 1, $1);}
;
FunctionBodyDecl: Expr {$$ = allocTree(FUNC_BODY_DECL, "func_body_decl", 1, $1);}
    | RETURN Expr      {$$ = allocTree(FUNC_BODY_DECL, "func_body_decl", 1, $1);}
;
    //| PatternBlock      {$$ = allocTree(FUNC_BODY_DECL, "func_body_decl", 1, $1);}
    //| CondStmts         {$$ = allocTree(FUNC_BODY_DECL, "func_body_decl", 1, $1);}


/* -- EXPRESSION GRAMMAR -- */

Expr: CondOrExpr {$$ = allocTree(EXPR, "expr", 1, $1);}
;
CondOrExpr: CondAndExpr                {$$ = allocTree(COND_OR_EXPR, "cond_or_expr", 1, $1);}
    | CondOrExpr LOGICALOR CondAndExpr {$$ = allocTree(COND_OR_EXPR, "cond_or_expr", 2, $1, $3);}
;
CondAndExpr: EqExpr                 {$$ = allocTree(COND_AND_EXPR, "cond_and_expr", 1, $1);}
    | CondAndExpr LOGICALAND EqExpr {$$ = allocTree(COND_AND_EXPR, "cond_and_expr", 2, $1, $3);}
;
EqExpr: RelationExpr                 {$$ = allocTree(EQ_EXPR, "eq_expr", 1, $1);}
    | EqExpr ISEQUALTO RelationExpr  {$$ = allocTree(EQ_EXPR, "eq_expr", 3, $1, $2, $3);}
    | EqExpr NOTEQUALTO RelationExpr {$$ = allocTree(EQ_EXPR, "eq_expr", 3, $1, $2, $3);}
;
RelationExpr: AddExpr                 {$$ = allocTree(RELATION_EXPR, "relation_expr", 1, $1);}
    | RelationExpr RelationOp AddExpr {$$ = allocTree(RELATION_EXPR, "relation_expr", 3, $1, $2, $3);} 
;
AddExpr: MultExpr               {$$ = allocTree(ADD_EXPR, "add_expr", 1, $1);}
    | AddExpr ADD MultExpr      {$$ = allocTree(ADD_EXPR, "add_expr", 3, $1, $2, $3);}
    | AddExpr SUBTRACT MultExpr {$$ = allocTree(ADD_EXPR, "add_expr", 3, $1, $2, $3);}
;
/*
MultExpr: ConcatExpr {$$ = allocTree(MULT_EXPR, "mult_expr", 1, $1);}
;
*/
MultExpr: UnaryExpr               {$$ = allocTree(MULT_EXPR, "mult_expr", 1, $1);}
    | MultExpr MULTIPLY UnaryExpr {$$ = allocTree(MULT_EXPR, "mult_expr", 3, $1, $2, $3);}
    | MultExpr DIVIDE UnaryExpr   {$$ = allocTree(MULT_EXPR, "mult_expr", 3, $1, $2, $3);}
    | MultExpr MODULO UnaryExpr   {$$ = allocTree(MULT_EXPR, "mult_expr", 3, $1, $2, $3);}
;
UnaryExpr: ConcatExpr    {$$ = allocTree(UNARY_EXPR, "unary_expr", 1, $1);}
    | NOT UnaryExpr      {$$ = allocTree(UNARY_EXPR, "unary_expr", 2, $1, $2);}
    | SUBTRACT UnaryExpr {$$ = allocTree(UNARY_EXPR, "unary_expr", 2, $1, $2);}
;
ConcatExpr: PostFixExpr          {$$ = allocTree(CONCAT_EXPR, "concat_expr", 1, $1);}
    | ConcatExpr BAR PostFixExpr {$$ = allocTree(CONCAT_EXPR, "concat_expr", 2, $1, $3);}
;
PostFixExpr: Name  {$$ = allocTree(POST_FIX_EXPR, "post_fix_expr", 1, $1);}
    | Primary      {$$ = allocTree(POST_FIX_EXPR, "post_fix_expr", 1, $1);}
;

/* -- NAMES AND FUNCTION CALLS -- */

Name: IDENTIFIER {$$ = allocTree(NAME, "name", 1, $1);}
;
Primary: Literal {$$ = allocTree(PRIMARY, "primary", 1, $1);}
;


/* -- TYPES AND LITERALS -- */

RelationOp: GREATERTHAN  {$$ = allocTree(RELATION_OP, "relation_op", 1, $1);}
    | GREATERTHANOREQUAL {$$ = allocTree(RELATION_OP, "relation_op", 1, $1);}
    | LESSTHAN           {$$ = allocTree(RELATION_OP, "relation_op", 1, $1);}
    | LESSTHANOREQUAL    {$$ = allocTree(RELATION_OP, "relation_op", 1, $1);}
;
Type: BOOLEAN  {$$ = allocTree(TYPE, "type", 1, $1);}
    | INT      {$$ = allocTree(TYPE, "type", 1, $1);}
    | FLOAT    {$$ = allocTree(TYPE, "type", 1, $1);}
    | CHAR     {$$ = allocTree(TYPE, "type", 1, $1);}
    | STRING   {$$ = allocTree(TYPE, "type", 1, $1);}
    | SYMBOL   {$$ = allocTree(TYPE, "type", 1, $1);}
    | FUNCTION {$$ = allocTree(TYPE, "type", 1, $1);}
    | LBRACKET Type RBRACKET    {$$ = allocTree(TYPE, "type", 3, $1, $2, $3);}
    | Name                      {$$ = allocTree(TYPE, "type", 1, $1);}
//    | LBRACE TupTypeDecl RBRACE {$$ = allocTree(TYPE, "type", 1, $2);}
;
Literal: LITERALBOOL  {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALINT      {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALFLOAT    {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALCHAR     {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALSTRING   {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALSYMBOL   {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALHEX      {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | ListLiteralsOpt {$$ = allocTree(LITERAL, "literal", 1, $1);}
;
ListLiteralsOpt: LBRACE ListLiterals RBRACE {$$ = allocTree(LIST_LITERALS_OPT, "list_literals_opt", 1, $2);}
| LBRACE RBRACE {$$ = NULL;}
;
ListLiterals: ListLiterals COMA Expr    {$$ = allocTree(LIST_LITERALS, "list_literals", 2, $1, $3);}
    | Expr            {$$ = allocTree(LIST_LITERALS, "list_literals", 1, $1);}
;

%%
