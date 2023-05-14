/*
 * File: tree.h
 * Author: Noe Garcia
 * Date: February, 2023
 * Description: Header file contains external variables used for the syntax
 *  tree of the Solace compiler. The following enumeration contains all
 *  integer values corresponding to terminal values. Finally, all token and
 *  tree structures are defined.
 */

extern char* yyfile; // !- contains file to compile -!

enum sol_terms {
    FUNCTION_DECL=1001,
    FUNCTION_HEADER,
    FUNCTION_BODY,
    FUNCTION_BODY_DECLS,
    FUNCTION_BODY_DECL, // 1005
    FUNCTION_RETURN_VAL,
    FIELD_DECL,
    TYPE,
    NAME,
    VAR_DECLS,          // 1010
    VAR_DECL,
    FORMAL_PARAM_LIST_OPT,
    FORMAL_PARAM_LIST,
    FORMAL_PARAM,
    ARG_LIST_OPT,       // 1015
    BLOCK,
    BLOCK_STMTS_OPT,
    BLOCK_STMTS,
    BLOCK_STMT,
    LOCAL_VAR_DECL_STMT,// 1020
    LOCAL_VAR_DECL,
    STMT_WT,
    STMT,
    EXPR_STMT,
    STMT_EXPR,          // 1025
    IF_THEN_STMT,
    IF_THEN_ELSE_STMT,
    ELSE_IF_SEQ,
    ELSE_IF_STMT,
    BREAK_STMT,         // 1030
    RETURN_STMT,
    PRIMARY,
    LITERAL,
    ARG_LIST,
    FIELD_ACCESS,       // 1035
    POST_FIX_EXPR,
    UNARY_EXPR,
    MULT_EXPR,
    ADD_EXPR,
    REL_OP,             // 1040
    REL_EXPR,
    EQ_EXPR,
    COND_AND_EXPR,
    COND_OR_EXPR,
    EXPR,               // 1045
    ASSIGN,
    LEFT_HAND_SIDE,
    ASSIGN_OP,
};