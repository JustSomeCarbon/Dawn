/* Definitions */

%{
    #include <stdio.h>
    #include "tree.h"
    extern int yylex(void);
    extern void yyerror(char const *);

    struct tree* root;
%}

%define parse.lac full      // required for verbose error reporting
%define parse.error verbose // required for detailed error messages

%union {
    struct tree* treeptr;
}

%token <treeptr> BOOLEAN INT FLOAT CHAR STRING SYMBOL HEADVAR TAILVAR
%token <treeptr> LITERALBOOL LITERALINT LITERALHEX LITERALFLOAT
%token <treeptr> LITERALCHAR LITERALSTRING LITERALSYMBOL FUNCTION
%token <treeptr> STRUCT ADD SUBTRACT MULTIPLY DIVIDE MODULO
%token <treeptr> ASSIGNMENT BAR ARROWOP RETURN SUBSCRIPT DOT
%token <treeptr> EQUALTO LBRACE RBRACE LPAREN RPAREN
%token <treeptr> LBRACKET RBRACKET COMA COLON SEMICOLON PACK MAINPACK
%token <treeptr> MAINFUNC IDENTIFIER RETURNTYPE USE DROPVAL
%token <treeptr> STRINGERR CHARERR COMMENTERR
%token <treeptr> ISEQUALTO NOTEQUALTO LOGICALAND LOGICALOR NOT INCREMENT DECREMENT
%token <treeptr> GREATERTHANOREQUAL LESSTHANOREQUAL GREATERTHAN LESSTHAN

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
%type <treeptr> FunctionCall
%type <treeptr> PatternBlocks
%type <treeptr> PatternBlock
%type <treeptr> PackName
%type <treeptr> PackNameCall
%type <treeptr> PackCallUnwrap
%type <treeptr> Type
%type <treeptr> Name
%type <treeptr> VarDecl
%type <treeptr> VarIdentifiers
%type <treeptr> StructVarDecl
%type <treeptr> StructVarParams
%type <treeptr> FormalParamListOpt
%type <treeptr> FormalParamList
%type <treeptr> FormalParam
%type <treeptr> ArgListOpt
%type <treeptr> ArgList
%type <treeptr> ArgVal
%type <treeptr> Primary
%type <treeptr> TuppleType
%type <treeptr> TuppleDecl
%type <treeptr> TuppleConst
%type <treeptr> Literals
%type <treeptr> Literal
%type <treeptr> FieldAccess
%type <treeptr> Field
%type <treeptr> PostFixExpr
%type <treeptr> UnaryExpr
%type <treeptr> MultExpr
%type <treeptr> AddExpr
%type <treeptr> RelOp
%type <treeptr> RelExpr
%type <treeptr> EqExpr
%type <treeptr> CondAndExpr
%type <treeptr> CondOrExpr
%type <treeptr> ConcatExprs
%type <treeptr> ConcatExpr
%type <treeptr> Expr
%type <treeptr> LeftHandSide
%type <treeptr> Assign
%type <treeptr> AssignOp

%%

/* Rules */

/* -- Source File/Package -- */

SourcePack: SourceFile { root = $1; };
SourceFile: PackDecl UseDecls DataDefDecls FunctionDecls
    {$$ = allocTree(SOURCE_FILE, "source_file", 4, $1, $2, $3, $4);}
;
PackDecl: PACK COLON MAINPACK {$$ = allocTree(PACK_DECL, "pack_decl", 2, $1, $3);}
    | PACK COLON PackName     {$$ = allocTree(PACK_DECL, "pack_decl", 2, $1, $3);}
;
UseDecls: UseDecls UseDecl  {$$ = allocTree(USE_DECLS, "use_decls", 2, $1, $2);}
    | UseDecl               {$$ = allocTree(USE_DECLS, "use_decls", 1, $1);}
;
UseDecl: USE PackName SEMICOLON {$$ = allocTree(USE_DECL, "use_decl", 2, $1, $2);}
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
StructDecl: STRUCT IDENTIFIER LBRACE StructBody RBRACE SEMICOLON
    {$$ = allocTree(STRUCT_DECL, "struct_decl", 3, $1, $2, $4);}
;
StructBody: StructParams {$$ = allocTree(STRUCT_PARAMS, "struct_params", 1, $1);}
;
StructParams: StructParams StructParam {$$ = allocTree(STRUCT_PARAMS, "struct_params", 2, $1, $2);}
    | StructParam {$$ = allocTree(STRUCT_PARAMS, "struct_params", 1, $1);}
;
StructParam: IDENTIFIER Type SEMICOLON {$$ = allocTree(STRUCT_PARAM, "struct_param", 2, $1, $2);}


/* -- Function Definitions -- */

FunctionDecls: FunctionDecls FunctionDecl
    {$$ = allocTree(FUNCTION_DECLS, "function_decls", 2, $1, $2);}
    | FunctionDecl {$$ = allocTree(FUNCTION_DECLS, "function_decls", 1, $1);}
;
FunctionDecl: FunctionHeader FunctionBody
    {$$ = allocTree(FUNCTION_DECL, "function_decl", 2, $1, $2);}
;
FunctionHeader: FUNCTION IDENTIFIER RETURNTYPE LPAREN FormalParamListOpt RPAREN
    {$$ = allocTree(FUNCTION_HEADER, "function_header", 3, $2, $3, $5);}
    | FUNCTION MAINFUNC RETURNTYPE LPAREN FormalParamListOpt RPAREN
    {$$ = allocTree(FUNCTION_HEADER, "function_header", 2, $3, $5);}
;
FormalParamListOpt: FormalParamList
    {$$ = allocTree(FORMAL_PARAM_LIST_OPT, "formal_param_list_opt", 1, $1);}
    | {$$ = NULL;}
;
FormalParamList: FormalParam
    {$$ = allocTree(FORMAL_PARAM_LIST, "formal_param_list", 1, $1);}
    | FormalParamList COMA FormalParam  {$$ = allocTree(FORMAL_PARAM_LIST, "formal_param_list", 2, $1, $3);}
;
FormalParam: IDENTIFIER Type {$$ = allocTree(FORMAL_PARAM, "formal_param", 2, $1, $2);}
    | LBRACKET HEADVAR BAR TAILVAR RBRACKET Type
    {$$ = allocTree(FORMAL_PARAM, "formal_param", 3, $2, $4, $6);}
;
ArgListOpt: ArgList {$$ = allocTree(ARG_LIST_OPT, "arg_list_opt", 1, $1);}
    | {$$ = NULL;}
;
ArgList: ArgList COMA ArgVal {$$ = allocTree(ARG_LIST, "arg_list", 2, $1, $3);}
    | ArgVal {$$ = allocTree(ARG_LIST, "arg_list", 1, $1);}
;
ArgVal: Expr     {$$ = allocTree(ARG_VAL, "arg_val", 1, $1);}
    | IDENTIFIER {$$ = allocTree(ARG_VAL, "arg_val", 1, $1);}
    | Literal    {$$ = allocTree(ARG_VAL, "arg_val", 1, $1);}
;


/* -- Function Body Definitions -- */

FunctionBody: LBRACE FunctionBodyDecls RBRACE
    {$$ = allocTree(FUNCTION_BODY, "function_body", 1, $2);}
    | LBRACE RBRACE {/* nothing in function */}
;
FunctionBodyDecls: FunctionBodyDecls FunctionBodyDecl
    {$$ = allocTree(FUNCTION_BODY_DECLS, "function_body_decls", 2, $1, $2);}
    | FunctionBodyDecl {$$ = allocTree(FUNCTION_BODY_DECLS, "function_body_decls", 2, $1);}
;
FunctionBodyDecl: FunctionReturnVal {$$ = allocTree(FUNCTION_BODY_DECL, "function_body_decl", 1, $1);}
    | VarDecl       {$$ = allocTree(FUNCTION_BODY_DECL, "function_body_decl", 1, $1);}
    | StructVarDecl {$$ = allocTree(FUNCTION_BODY_DECL, "function_body_decl", 1, $1);}
    | FunctionCall  {$$ = allocTree(FUNCTION_BODY_DECL, "function_body_decl", 1, $1);}
    | PackNameCall  {$$ = allocTree(FUNCTION_BODY_DECL, "function_body_decl", 1, $1);}
    | LBRACE PatternBlocks RBRACE
    {$$ = allocTree(FUNCTION_BODY_DECL, "function_body_decl", 1, $2);}
;
FunctionReturnVal: IDENTIFIER SEMICOLON {$$ = allocTree(FUNCTION_RETURN_VAL, "function_return_val", 1, $1);}
    | Literal SEMICOLON           {$$ = allocTree(FUNCTION_RETURN_VAL, "function_return_val", 1, $1);}
    | Expr SEMICOLON              {$$ = allocTree(FUNCTION_RETURN_VAL, "function_return_val", 1, $1);}
    | RETURN IDENTIFIER SEMICOLON {$$ = allocTree(FUNCTION_RETURN_VAL, "function_return_val", 1, $2);}
    | RETURN Literal SEMICOLON    {$$ = allocTree(FUNCTION_RETURN_VAL, "function_return_val", 1, $2);}
    | RETURN Expr SEMICOLON       {$$ = allocTree(FUNCTION_RETURN_VAL, "function_return_val", 1, $2);}
;
FunctionCall: IDENTIFIER LPAREN ArgListOpt RPAREN SEMICOLON
    {$$ = allocTree(FUNCTION_CALL, "function_call", 2, $1, $3);}
;
PatternBlocks: PatternBlock          {$$ = allocTree(PATTERN_BLOCKS, "pattern_blocks", 1, $1);}
    | PatternBlocks BAR PatternBlock {$$ = allocTree(PATTERN_BLOCKS, "pattern_blocks", 2, $1, $3);}
;
PatternBlock: LPAREN Expr RPAREN ARROWOP FunctionBodyDecls
    {$$ = allocTree(PATTERN_BLOCK, "pattern_block", 2, $2, $5);}
    | Expr ARROWOP FunctionBodyDecls
    {$$ = allocTree(PATTERN_BLOCK, "pattern_block", 2, $1, $3);}
;
PackName: IDENTIFIER {$$ = allocTree(PACK_NAME, "pack_name", 1, $1);}
;
PackNameCall: PackNameCall SUBSCRIPT PackCallUnwrap ArgListOpt {$$ = allocTree(PACK_NAME_CALL, "pack_name_call", 3, $1, $3, $4);}
    | PackCallUnwrap ArgListOpt {$$ = allocTree(PACK_NAME_CALL, "pack_name_call", 2, $1, $2);}
;
PackCallUnwrap: IDENTIFIER SUBSCRIPT IDENTIFIER {$$ = allocTree(PACK_CALL_UNWRAP, "pack_call_unwrap", 2, $1, $3);}
;


/* -- Variable Definitions & Assignments -- */

VarDecl: VarIdentifiers Type ASSIGNMENT Literals SEMICOLON  {$$ = allocTree(VAR_DECL, "var_decl", 3, $1, $2, $4);}
    | IDENTIFIER Type ASSIGNMENT FunctionCall     {$$ = allocTree(VAR_DECL, "var_decl", 3, $1, $2, $4);}
    | VarIdentifiers Type ASSIGNMENT FunctionCall {$$ = allocTree(VAR_DECL, "var_decl", 3, $1, $2, $4);}
    | IDENTIFIER Type ASSIGNMENT ConcatExprs SEMICOLON      {$$ = allocTree(VAR_DECL, "var_decl", 3, $1, $2, $4);}
    | IDENTIFIER Type ASSIGNMENT PackNameCall SEMICOLON     {$$ = allocTree(VAR_DECL, "var_decl", 3, $1, $2, $4);}
    | VarIdentifiers Type ASSIGNMENT PackNameCall SEMICOLON {$$ = allocTree(VAR_DECL, "var_decl", 3, $1, $2, $4);}
    | DROPVAL ASSIGNMENT Literals SEMICOLON       {$$ = allocTree(VAR_DECL, "var_decl", 1, $3);}
    | DROPVAL ASSIGNMENT FunctionCall             {$$ = allocTree(VAR_DECL, "var_decl", 1, $3);}
    | DROPVAL ASSIGNMENT PackNameCall SEMICOLON   {$$ = allocTree(VAR_DECL, "var_decl", 1, $3);}
;
VarIdentifiers: VarIdentifiers COMA IDENTIFIER
    {$$ = allocTree(VAR_IDENTIFIERS, "var_identifiers", 2, $1, $3);}
    | IDENTIFIER {$$ = allocTree(VAR_IDENTIFIERS, "var_identifiers", 1, $1);}
;
StructVarDecl: STRUCT IDENTIFIER EQUALTO LBRACKET StructVarParams RBRACKET
    {$$ = allocTree(STRUCT_VAR_DECL, "struct_var_decl", 2, $2, $5);}
;
StructVarParams: StructVarParams COMA Literal {$$ = allocTree(STRUCT_VAR_PARAMS, "struct_var_params", 2, $1, $3);}
    | StructVarParams COMA IDENTIFIER {$$ = allocTree(STRUCT_VAR_PARAMS, "struct_var_params", 2, $1, $3);}
    | StructVarParams COMA IDENTIFIER EQUALTO Literal
    {$$ = allocTree(STRUCT_VAR_PARAMS, "struct_var_params", 2, $1, $3);}
    | StructVarParams COMA IDENTIFIER EQUALTO IDENTIFIER
    {$$ = allocTree(STRUCT_VAR_PARAMS, "struct_var_params", 2, $1, $3);}
    | Literal    {$$ = allocTree(STRUCT_VAR_PARAMS, "struct_var_params", 1, $1);}
    | IDENTIFIER {$$ = allocTree(STRUCT_VAR_PARAMS, "struct_var_params", 1, $1);}
;


/* -- Operation and Name Definitions -- */

Name: IDENTIFIER    {$$ = allocTree(NAME, "name", 1, $1);}
    | PackNameCall {$$ = allocTree(NAME, "name", 1, $1);}
;
FieldAccess: Field          {$$ = allocTree(FIELD_ACCESS, "field_access", 1, $1);}
    | FieldAccess DOT Field {$$ = allocTree(FIELD_ACCESS, "field_access", 2, $1, $3);}
;
Field: IDENTIFIER DOT IDENTIFIER {$$ = allocTree(FIELD, "field", 2, $1, $3);}
;
CondOrExpr: CondAndExpr {$$ = allocTree(COND_OR_EXPR, "cond_or_expr", 1, $1);}
    | CondOrExpr LOGICALOR CondAndExpr {$$ = allocTree(COND_OR_EXPR, "cond_or_expr", 2, $1, $3);}
;
CondAndExpr: EqExpr {$$ = allocTree(COND_AND_EXPR, "cond_and_expr", 1, $1);}
    | CondAndExpr LOGICALAND EqExpr {$$ = allocTree(COND_AND_EXPR, "cond_and_expr", 2, $1, $3);}
;
EqExpr: RelExpr {$$ = allocTree(EQ_EXPR, "eq_expr", 1, $1);}
    | EqExpr ISEQUALTO RelExpr  {$$ = allocTree(EQ_EXPR, "eq_expr", 2, $1, $3);}
    | EqExpr NOTEQUALTO RelExpr {$$ = allocTree(EQ_EXPR, "eq_expr", 2, $1, $3);}
;
RelExpr: AddExpr {$$ = allocTree(REL_EXPR, "rel_expr", 1, $1);}
    | RelExpr RelOp AddExpr {$$ = allocTree(REL_EXPR, "rel_expr", 3, $1, $2, $3);}
;
RelOp: LESSTHANOREQUAL   {$$ = allocTree(REL_OP, "rel_op", 1, $1);}
    | GREATERTHANOREQUAL {$$ = allocTree(REL_OP, "rel_op", 1, $1);}
    | LESSTHAN           {$$ = allocTree(REL_OP, "rel_op", 1, $1);}
    | GREATERTHAN        {$$ = allocTree(REL_OP, "rel_op", 1, $1);}
;
AddExpr: MultExpr {$$ = allocTree(ADD_EXPR, "add_expr", 1, $1);}
    | AddExpr ADD MultExpr      {$$ = allocTree(ADD_EXPR, "add_expr", 1, $1);}
    | AddExpr SUBTRACT MultExpr {$$ = allocTree(ADD_EXPR, "add_expr", 1, $1);}
;
MultExpr: UnaryExpr {$$ = allocTree(MULT_EXPR, "mult_expr", 1, $1);}
    | MultExpr MULTIPLY UnaryExpr {$$ = allocTree(MULT_EXPR, "mult_expr", 2, $1, $3);}
    | MultExpr DIVIDE UnaryExpr   {$$ = allocTree(MULT_EXPR, "mult_expr", 2, $1, $3);}
    | MultExpr MODULO UnaryExpr   {$$ = allocTree(MULT_EXPR, "mult_expr", 2, $1, $3);}
;
UnaryExpr: SUBTRACT UnaryExpr {$$ = allocTree(UNARY_EXPR, "unary_expr", 1, $2);}
    | NOT UnaryExpr {$$ = allocTree(UNARY_EXPR, "unary_expr", 1, $2);}
    | PostFixExpr   {$$ = allocTree(UNARY_EXPR, "unary_expr", 1, $1);}
;
PostFixExpr: Primary {$$ = allocTree(POST_FIX_EXPR, "post_fix_expr", 1, $1);}
    | Name {$$ = allocTree(POST_FIX_EXPR, "post_fix_expr", 1, $1);}
;
ConcatExprs: ConcatExprs BAR ConcatExpr {$$ = allocTree(CONCAT_EXPRS, "concat_exprs", 2, $1, $3);}
    | ConcatExpr {$$ = allocTree(CONCAT_EXPRS, "concat_exprs", 1, $1);}
;
ConcatExpr: LITERALSTRING BAR LITERALSTRING {$$ = allocTree(CONCAT_EXPR, "concat_expr", 2, $1, $3);}
    | LITERALSTRING BAR LITERALCHAR {$$ = allocTree(CONCAT_EXPR, "concat_expr", 2, $1, $3);}
    | LITERALCHAR BAR LITERALSTRING {$$ = allocTree(CONCAT_EXPR, "concat_expr", 2, $1, $3);}
    | LITERALCHAR BAR LITERALCHAR   {$$ = allocTree(CONCAT_EXPR, "concat_expr", 2, $1, $3);}
;
Expr: CondOrExpr {$$ = allocTree(EXPR, "expr", 1, $1);}
    | Assign {$$ = allocTree(EXPR, "expr", 1, $1);}
;
Assign: LeftHandSide AssignOp Expr {$$ = allocTree(ASSIGN, "assign", 3, $1, $2, $3);}
;
AssignOp: ASSIGNMENT {$$ = allocTree(ASSIGN_OP, "assign_op", 1, $1);}
    | INCREMENT  {$$ = allocTree(ASSIGN_OP, "assign_op", 1, $1);}
    | DECREMENT  {$$ = allocTree(ASSIGN_OP, "assign_op", 1, $1);}
;
LeftHandSide: Name {$$ = allocTree(LEFT_HAND_SIDE, "left_hand_side", 1, $1);}
    | FieldAccess  {$$ = allocTree(LEFT_HAND_SIDE, "left_hand_side", 1, $1);}
;


/* -- Type Definitions -- */

Primary: Literal         {$$ = allocTree(PRIMARY, "primary", 1, $1);}
    | LPAREN Expr RPAREN {$$ = allocTree(PRIMARY, "primary", 1, $2);}
    | Name               {$$ = allocTree(PRIMARY, "primary", 1, $1);}
    | PackNameCall       {$$ = allocTree(PRIMARY, "primary", 1, $1);}
    | FunctionCall       {$$ = allocTree(PRIMARY, "primary", 1, $1);}
;
Type: INT      {$$ = allocTree(TYPE, "type", 1, $1);}
    | FLOAT    {$$ = allocTree(TYPE, "type", 1, $1);}
    | BOOLEAN  {$$ = allocTree(TYPE, "type", 1, $1);}
    | STRING   {$$ = allocTree(TYPE, "type", 1, $1);}
    | CHAR     {$$ = allocTree(TYPE, "type", 1, $1);}
    | SYMBOL   {$$ = allocTree(TYPE, "type", 1, $1);}
    | FUNCTION {$$ = allocTree(TYPE, "type", 1, $1);}
    | LBRACKET Type RBRACKET {$$ = allocTree(TYPE, "type", 1, $2);}
;
TuppleType: LBRACE TuppleDecl RBRACE {$$ = allocTtree(TUPPLE_TYPE, "tupple_type", 1, $2);}
    | LBRACE RBRACE {/* nothing in function */}
;
TuppleDecl: TuppleDecl COMA Literal {$$ = allocTtree(TUPPLE_DECL, "tupple_decl", 2, $1, $3);}
    | TuppleDecl COMA IDENTIFIER    {$$ = allocTree(TUPPLE_DECL, "tupple_decl", 2, $1, $3);}
    | Literal    {$$ = allocTree(TUPPLE_DECL, "tupple_decl", 1, $1);}
    | IDENTIFIER {$$ = allocTree(TUPPLE_DECL, "tupple_decl", 1, $1);}
;
TuppleConst: TuppleConst COMA Type {$$ = allocTtree(TUPPLE_CONST, "tupple_const", 2, $1, $3);}
;
Literals: Literals Literal {$$ = allocTree(LITERALS, "literals", 2, $1, $2);}
    | Literal              {$$ = allocTree(LITERALS, "literals", 1, $1);}
;
Literal: LITERALINT {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALBOOL   {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALFLOAT  {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALHEX    {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALSTRING {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALCHAR   {$$ = allocTree(LITERAL, "literal", 1, $1);}
    | LITERALSYMBOL {$$ = allocTree(LITERAL, "literal", 1, $1);}
;

%%

/* Auxiliary Routines  */
// N/A
