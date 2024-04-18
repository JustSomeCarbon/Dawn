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
%token <treeptr> MAINFUNC IDENTIFIER USE DROPVAL DOT SELF
%token <treeptr> BOOLEAN INT FLOAT CHAR STRING SYMBOL
%token <treeptr> LITERALBOOL LITERALINT LITERALHEX LITERALFLOAT
%token <treeptr> LITERALCHAR LITERALSTRING LITERALSYMBOL FUNCTION
%token <treeptr> STRUCT ADD SUBTRACT MULTIPLY DIVIDE MODULO
%token <treeptr> ASSIGNMENT BAR ARROWOP RETURN
%token <treeptr> ISEQUALTO NOTEQUALTO LOGICALAND LOGICALOR NOT
%token <treeptr> GREATERTHANOREQUAL LESSTHANOREQUAL GREATERTHAN LESSTHAN

%type <treeptr> FileRoot
%type <treeptr> SourceSpace
%type <treeptr> FileDefinitions

%type <treeptr> UseDefinition
%type <treeptr> ImportList

%type <treeptr> StructDefinition
%type <treeptr> StructParams
%type <treeptr> StructParam

%type <treeptr> FunctionDefinition
%type <treeptr> FunctionHeader
%type <treeptr> ParameterListOpt
%type <treeptr> ParameterList
%type <treeptr> Parameter
%type <treeptr> FunctionBody
%type <treeptr> FunctionBodyDecls
%type <treeptr> FunctionBodyDecl

%type <treeptr> PatternBlock
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

FileRoot: SourceSpace FileDefinitions {root = allocTree(FILE_ROOT, "file_root", 2, $1, $2);}
    | SourceSpace                     {root = allocTree(FILE_ROOT, "file_root", 1, $1);}
;
SourceSpace: MODSPACE COLON Name SEMICOLON {$$ = allocTree(SOURCE_SPACE, "source_space", 1, $3);}
;
FileDefinitions: FileDefinitions StructDefinition {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 2, $1, $2);}
    | FileDefinitions UseDefinition      {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 2, $1, $2);}
    | FileDefinitions FunctionDefinition {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 2, $1, $2);}
    | UseDefinition      {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 1, $1);}
    | StructDefinition   {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 1, $1);}
    | FunctionDefinition {$$ = allocTree(FILE_DEFINITIONS, "file_definitions", 1, $1);}
;

/*  -- USE GRAMAR DEFINITIONS -- */

UseDefinition: USE Name SEMICOLON                       {$$ = allocTree(USE_DEFINITIONS, "use_definitions", 1, $2);}
    | USE Name COLON LBRACE ImportList RBRACE SEMICOLON {$$ = allocTree(USE_DEFINITIONS, "use_definitions", 2, $2, $5);}
;
ImportList: ImportList COMA Name {$$ = allocTree(IMPORT_LIST, "import_list", 2, $1, $3);}
    | Name {$$ = allocTree(IMPORT_LIST, "import_list", 1, $1);}
;


/* -- STRUCT GRAMAR DEFINITIONS -- */

StructDefinition: STRUCT Name LBRACE StructParams RBRACE SEMICOLON {$$ = allocTree(STRUCT_DEFINITION, "struct_definition", 2, $2, $4);}
;
StructParams: StructParams StructParam {$$ = allocTree(STRUCT_PARAMS, "struct_params", 2, $1, $2);}
    | StructParam {$$ = allocTree(STRUCT_PARAMS, "struct_params", 1, $1);}
;
StructParam: Name Type SEMICOLON {$$ = allocTree(STRUCT_PARAM, "struct_param", 2, $1, $2);}
;


/*  -- FUNCTION DECLARATION GRAMAR DEFINITIONS -- */

FunctionDefinition: FunctionHeader FunctionBody {$$ = allocTree(FUNC_DEFINITION, "func_definition", 2, $1, $2);}
;
FunctionHeader: FUNCTION Name Type ParameterListOpt {$$ = allocTree(FUNC_HEADER, "func_header", 3, $2, $3, $4);}
    | FUNCTION MAINFUNC Type ParameterListOpt       {$$ = allocTree(FUNC_HEADER, "func_header", 3, $2, $3, $4);}
;
ParameterListOpt: LPAREN ParameterList RPAREN {$$ = allocTree(PARAM_LIST_OPT, "param_list_opt", 1, $2);}
    | LPAREN RPAREN {$$ = NULL;}
;
ParameterList: ParameterList COMA Parameter {$$ = allocTree(PARAM_LIST, "param_list", 2, $1, $3);}
    | Parameter {$$ = allocTree(PARAM_LIST, "param_list", 1, $1);}
;
Parameter: Name Type {$$ = allocTree(PARAM, "parameter", 2, $1, $2);}
;
FunctionBody: LBRACE FunctionBodyDecls RBRACE {$$ = allocTree(FUNC_BODY, "func_body", 1, $2);}
    | LBRACE RBRACE {$$ = NULL;}
;
FunctionBodyDecls: FunctionBodyDecls FunctionBodyDecl {$$ = allocTree(FUNC_BODY_DECLS, "func_body_decls", 2, $1, $2);}
    | FunctionBodyDecl {$$ = allocTree(FUNC_BODY_DECLS, "func_body_decls", 1, $1);}
;
FunctionBodyDecl: LBRACE PatternBlock RBRACE {$$ = allocTree(FUNC_BODY_DECL, "func_body_decl", 1, $2);}
    | Expr SEMICOLON             {$$ = allocTree(FUNC_BODY_DECL, "func_body_decl", 1, $1);}
    | RETURN Expr SEMICOLON      {$$ = allocTree(FUNC_BODY_DECL, "func_body_decl", 2, $1, $2);}
;


/* -- PATTERN EXPRESSION GRAMMAR -- */

PatternBlock: PatternBlock BAR PatternStmt {$$ = allocTree(PATTERN_BLOCK, "pattern_block", 2, $1, $3);}
    | PatternStmt {$$ = allocTree(PATTERN_BLOCK, "pattern_block", 1, $1);}
;
PatternStmt: Expr ARROWOP FunctionBodyDecls {$$ = allocTree(PATTERN_STMT, "pattern_stmt", 2, $1, $3);}
    | ARROWOP FunctionBodyDecls {$$ = allocTree(PATTERN_STMT, "pattern_stmt", 1, $2);}
;


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