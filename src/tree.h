/*
 * File: tree.h
 * Author: Noe Garcia
 * Date: February, 2023
 * Description: Header file contains external variables used for the syntax
 *  tree of the Solace compiler. The following enumeration contains all
 *  integer values corresponding to terminal values. Finally, all token and
 *  tree structures are defined.
 */

enum sol_terms {
    FUNCTION_DECL=1001,
    FUNCTION_HEADER,
    FUNCTION_BODY,
    FUNCTION_BODY_DECLS,
    FUNCTION_BODY_DECL,
    FUNCTION_RETURN_VAL,
    FIELD_DECL,
    TYPE,
    NAME,
    VAR_DECLS,
    VAR_DECL,
    FORMAL_PARAM_LIST_OPT,
    FORMAL_PARAM_LIST,
    FORMAL_PARAM,
    ARG_LIST_OPT,
    BLOCK,
    BLOCK_STMTS_OPT,
    BLOCK_STMTS,
    BLOCK_STMT,
    LOCAL_VAR_DECL_STMT,
    LOCAL_VAR_DECL,
    STMT_WT,
    STMT,
    EXPR_STMT,
    STMT_EXPR,
    IF_THEN_STMT,
    IF_THEN_ELSE_STMT,
    ELSE_IF_SEQ,
    ELSE_IF_STMT,
    BREAK_STMT,
    RETURN_STMT,
    PRIMARY,
    LITERAL,
    ARG_LIST,
    FIELD_ACCESS,
    POST_FIX_EXPR,
    UNARY_EXPR,
    MULT_EXPR,
    ADD_EXPR,
    REL_OP,
    REL_EXPR,
    EQ_EXPR,
    COND_AND_EXPR,
    COND_OR_EXPR,
    EXPR,
    ASSIGN,
    LEFT_HAND_SIDE,
    ASSIGN_OP,
};

int alloc_token(int code);