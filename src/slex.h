#ifndef SLEX_H
#define SLEX_H

/*
 * The token structure used to retain the tokenized source file
 */
struct token {
    char* literal;
    int lineno;
    int category;
    char* fileorigin;
    literalValue val;
};

/*
 * The true value of the allocated token
 */
union literalValue {
    int ival;
    double dval;
    char* sval;
};

/*
 * The stack that contains all the tokens that have
 * been lexed
 */
struct tokenStack {
    token tok;
    tokenStack* next;
};

struct tokenStack* lex_source_file(char* file_name);

#endif