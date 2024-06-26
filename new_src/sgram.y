// Solace Grammar yacc definition file

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

%token <treeptr> LBRACKET RBRACKET COMA COLON SEMICOLON MODSPACE
%token <treeptr> LBRACE RBRACE LPAREN RPAREN
%token <treeptr> MAINFUNC IDENTIFIER USE DROPVAL DOT SELF ON DO END
%token <treeptr> BOOLEAN INT FLOAT CHAR STRING SYMBOL FUNCTION
%token <treeptr> THEN IF ELSEIF ELSE ENDOR MATCH
%token <treeptr> LITERALBOOL LITERALINT LITERALHEX LITERALFLOAT
%token <treeptr> LITERALCHAR LITERALSTRING LITERALSYMBOL DEFINE
%token <treeptr> STRUCT ADD SUBTRACT MULTIPLY DIVIDE MODULO
%token <treeptr> ASSIGNMENT BAR ARROWOP MATCHTO
%token <treeptr> ISEQUALTO NOTEQUALTO LOGICALAND LOGICALOR NOT
%token <treeptr> GREATERTHANOREQUAL LESSTHANOREQUAL GREATERTHAN LESSTHAN

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
%type <treeptr> FunctionBody
%type <treeptr> FunctionBodyPattern
%type <treeptr> FunctionBodyDecls
%type <treeptr> FunctionBodyDecl

%type <treeptr> PatternBlock
%type <treeptr> PatternStmts
%type <treeptr> PatternStmt

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

%%

/* Gramatical rules */

/* -- SOURCE FILE DEFINITION -- */

FileRoot: ModSpace ModSpaceDefinitions END {root = allocTree(FILE_ROOT, "file_root", 2, $1, $2);}
    | ModSpace END                    {root = allocTree(FILE_ROOT, "file_root", 1, $1);}
;
ModSpace: MODSPACE Name {$$ = allocTree(MOD_SPACE, "source_space", 1, $3);}
;
ModSpaceDefinitions: ModSpace ModSpaceDefinitions END {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 2, $1, $2);}
    | ModSpaceDefinitions DEFINE StructDefinition   {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 2, $1, $3);}
    | ModSpaceDefinitions UseDefinition      {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 2, $1, $2);}
    | ModSpaceDefinitions DEFINE FunctionDefinition {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 2, $1, $3);}
    | UseDefinition      {$$ = allocTree(MOD_SPACE_DEFINITIONS, "mod_space_definitions", 1, $1);}
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


/*  -- FUNCTION DECLARATION GRAMAR DEFINITIONS -- */

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
FunctionBody: DO FunctionBodyDecls END {$$ = allocTree(FUNC_BODY, "func_body", 1, $2);}
    | DO FunctionBodyDecls ENDOR FunctionBodyPattern {$$ = allocTree(FUNC_BODY, "func_body", 2, $2, $4)}
    | DO END {$$ = NULL;}
;
FunctionBodyPattern: ParameterListOpt Type FunctionBody {$$ = allocTree(FUNC_BODY_PATTERN, "func_body_pattern", 3, $1, $2, $3);}
;
FunctionBodyDecls: FunctionBodyDecls FunctionBodyDecl {$$ = allocTree(FUNC_BODY_DECLS, "func_body_decls", 2, $1, $2);}
    | FunctionBodyDecl {$$ = allocTree(FUNC_BODY_DECLS, "func_body_decls", 1, $1);}
;
FunctionBodyDecl: Expr  {$$ = allocTree(FUNC_BODY_DECL, "func_body_decl", 1, $1);}
    | Expr SEMICOLON    {$$ = allocTree(FUNC_BODY_DECL, "func_body_decl", 2, $1, $2);}
    | PatternBlock      {$$ = allocTree(FUNC_BODY_DECL, "func_body_decl", 1, $2);}
;


/* -- PATTERN EXPRESSION GRAMMAR -- */

PatternBlock: ON Expr MATCH PatternStmts END {$$ = allocTree(PATTERN_BLOCK, "pattern_block", 2, $2, $4);}
;
PatternStmts: PatternStmts PatternStmt {$$ = allocTree(PATTERN_STMTS, "pattern_stmts", 2, $1, $2);}
    | PatternStmt {$$ = allocTree(PATTERN_STMTS, "pattern_stmts", 1, $1);}
;
PatternStmt: Expr ARROWOP FunctionBodyDecls    {$$ = allocTree(PATTERN_STMT, "pattern_stmt", 2, $1, $3);}
    | DROPVAL ARROWOP FunctionBodyDecls {$$ = allocTree(PATTERN_STMT, "pattern_stmt", 1, $2);}
;


/* TODO -> */

/* CONDITION GRAMAR DEFINITIONS */
// THINGS


/* -- EXPRESSION GRAMAR DEFINITIONS -- */

Expr: CondOrExpr    {$$ = allocTree(EXPR, "expr", 1, $1);}
    | VarAssignment {$$ = allocTree(EXPR, "expr", 1, $1);}
;
CondOrExpr: CondOrExpr LOGICALOR CondAndExpr {$$ = allocTree(COND_OR_EXPR, "cond_or_expr", 2, $1, $3);}
    | CondAndExpr {$$ = allocTree(COND_OR_EXPR, "cond_or_expr", 1, $1);}
;
CondAndExpr: CondAndExpr LOGICALAND EqExpr {$$ = allocTree(COND_AND_EXPR, "cond_and_expr", 2, $1, $3);}
    | EqExpr {$$ = allocTree(COND_AND_EXPR, "cond_and_expr", 1, $1);}
;
EqExpr: EqExpr ISEQUALTO RelationExpr  {$$ = allocTree(EQ_EXPR, "eq_expr", 3, $1, $2, $3);}
    | EqExpr NOTEQUALTO RelationExpr   {$$ = allocTree(EQ_EXPR, "eq_expr", 3, $1, $2, $3);}
    | RelationExpr                     {$$ = allocTree(EQ_EXPR, "eq_expr", 1, $1);}
;
RelationExpr: RelationExpr RelationOp AddExpr {$$ = allocTree(RELATION_EXPR, "relation_expr", 3, $1, $2, $3);} 
    | AddExpr {$$ = allocTree(RELATION_EXPR, "relation_expr", 1, $1);}
;
AddExpr: AddExpr ADD MultExpr   {$$ = allocTree(ADD_EXPR, "add_expr", 3, $1, $2, $3);}
    | AddExpr SUBTRACT MultExpr {$$ = allocTree(ADD_EXPR, "add_expr", 3, $1, $2, $3);}
    | MultExpr                  {$$ = allocTree(ADD_EXPR, "add_expr", 1, $1);}
;
MultExpr: MultExpr MULTIPLY UnaryExpr {$$ = allocTree(MULT_EXPR, "mult_expr", 3, $1, $2, $3);}
    | MultExpr DIVIDE UnaryExpr       {$$ = allocTree(MULT_EXPR, "mult_expr", 3, $1, $2, $3);}
    | MultExpr MODULO UnaryExpr       {$$ = allocTree(MULT_EXPR, "mult_expr", 3, $1, $2, $3);}
    | UnaryExpr                       {$$ = allocTree(MULT_EXPR, "mult_expr", 1, $1);}
;
UnaryExpr: NOT UnaryExpr {$$ = allocTree(UNARY_EXPR, "unary_expr", 2, $1, $2);}
    | SUBTRACT UnaryExpr {$$ = allocTree(UNARY_EXPR, "unary_expr", 2, $1, $2);}
    | ConcatExpr         {$$ = allocTree(UNARY_EXPR, "unary_expr", 1, $1);}
;
ConcatExpr: ConcatExpr BAR PostFixExpr {$$ = allocTree(CONCAT_EXPR, "concat_expr", 2, $1, $3);}
    | PostFixExpr {$$ = allocTree(CONCAT_EXPR, "concat_expr", 1, $1);}
;
PostFixExpr: Primary   {$$ = allocTree(POST_FIX_EXPR, "post_fix_expr", 1, $1);}
    | Name             {$$ = allocTree(POST_FIX_EXPR, "post_fix_expr", 1, $1);}
;


/* -- ASSIGNMENT AND OPERATION GRAMAR DEFINITIONS --  */

VarAssignment: LeftHandSide ASSIGNMENT Expr {$$ = allocTree(VAR_ASSIGNMENT, "var_assignment", 2, $1, $3);}
;
LeftHandSide: Name Type {$$ = allocTree(LEFT_HAND_SIDE, "left_hand_side", 2, $1, $2);}
    | DROPVAL {$$ = allocTree(LEFT_HAND_SIDE, "left_hand_side", 1, $1);}
;


/* -- NAMES AND FUNCTION CALLS GRAMMAR DEFINITIONS -- */

Name: Name COLON IDENTIFIER {$$ = allocTree(NAME, "name", 3, $1, $2, $3);}
    | Name DOT IDENTIFIER   {$$ = allocTree(NAME, "name", 3, $1, $2, $3);}
    | Name FieldAccess      {$$ = allocTree(NAME, "name", 2, $1, $2);}
    | IDENTIFIER            {$$ = allocTree(NAME, "name", 1, $1);}
;
Primary: Literal         {$$ = allocTree(PRIMARY, "primary", 1, $1);}
    | LPAREN Expr RPAREN {$$ = allocTree(PRIMARY, "primary", 1, $2);}
    | LambdaExpr {$$ = allocTree(PRIMARY, "primary", 1, $1);}
    | FunctionCall       {$$ = allocTree(PRIMARY, "primary", 1, $1);}
;
FieldAccess: LBRACKET LITERALINT RBRACKET {$$ = allocTree(FIELD_ACCESS, "field_access", 1, $2);}
;
FunctionCall: Name ArgumentListOpt {$$ = allocTree(FUNC_CALL, "func_call", 2, $1, $2);}
    | SELF ArgumentListOpt         {$$ = allocTree(FUNC_CALL, "func_call", 2, $1, $2);}
;
LambdaExpr: ParameterListOpt Type ARROWOP FunctionBody {$$ = allocTree(LAMBDA_EXPR, "lambda_expr", 3, $1, $2, $4);}
;
ArgumentListOpt: LPAREN ArgumentList RPAREN {$$ = allocTree(ARG_LIST_OPT, "arg_list_opt", 1, $2);}
    | LPAREN RPAREN {$$ = NULL;}
;
ArgumentList: ArgumentList COMA Expr {$$ = allocTree(ARG_LIST, "arg_list", 2, $1, $3);}
    | Expr {$$ = allocTree(ARG_LIST, "arg_list", 1, $1);}
;


/*  -- TYPES AND LITERALS GRAMAR DEFINITIONS --  */

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
    | LBRACE TupTypeDecl RBRACE {$$ = allocTree(TYPE, "type", 1, $2);}
;
TupTypeDecl: TupTypeDecl COMA Type {$$ = allocTree(TUP_TYPE_DECL, "tup_type_decl", 2, $1, $3);}
    | Type {$$ = allocTree(TUP_TYPE_DECL, "tup_type_decl", 1, $1);}
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